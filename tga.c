#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* const tga = "font.tga";
const char* const signs = "signs.txt";

int main(){
	int font_width, font_height, font_v_spacer, font_h_spacer, tga_width, tga_pre, tga_height, symbols;
	FILE* inFile = fopen(tga, "rb");
	FILE* signFile = fopen(signs, "rb");
	char line[30];
	fgets(line, 30, signFile);
	sscanf(line, "%d %d %d %d", &font_width, &font_height, &font_v_spacer, &font_h_spacer);
	fgets(line, 30, signFile);
	sscanf(line, "%d %d %d", &tga_pre, &tga_width, &tga_height);
	fgets(line, 30, signFile);
	symbols = atoi(line);

	char a[tga_width][tga_height];
	for(int i = 0; i<tga_pre; ++i) fgetc(inFile);
	for(int i = 0; i<tga_width*tga_height; ++i) {
		if(fgetc(inFile) == 0) a[i%tga_width][tga_height-1-i/tga_width] = 1;
		else a[i%tga_width][tga_height-1-i/tga_width] = 0;
                fgetc(inFile);
                fgetc(inFile);
                fgetc(inFile);
	}
	
	printf("#ifndef ALPHABET_H\n#define ALPHABET_H\n#include <string.h>\n#include <stdlib.h>\nconst int WIDTH = %d;\nconst int HEIGHT = %d;\nstruct point{const int x; const int y;};\nstruct character{const char* name; const struct point* data; const int data_length;};\n\n", font_width, font_height);
	for(int i = 0; i < symbols; ++i) {
		fgets(line, 30, signFile);
		line[strlen(line)-1]=0;
		printf("const char n__%d[]=\"%s\";\nconst struct point p__%d[]={", i, line, i);
		int x = i%16 * (font_width + font_h_spacer);
		int y = i/16 * (font_height + font_v_spacer);
		for(int j = 0; j < font_height; ++j)
			for(int k = 0; k < font_width; ++k)
				if(a[x+k][y+j]) printf("{%d,%d},", k, j); 
		printf("};\n");
	}
	printf("\nstruct character alphabet[]={\n");
	for(int i = 0; i < symbols; ++i) {
		printf("{n__%d, p__%d, sizeof(p__%d)/sizeof(struct point)},\n", i, i, i);
	}
	printf("};\nint m_cmp(const void* a, const void* b){\n    return strcmp(*(const char**)a, *(const char**)b);\n}\n\nchar sorted = 0;\nvoid init_data(){\n    qsort(alphabet, sizeof(alphabet)/sizeof(struct character), sizeof(struct character), m_cmp);\n    sorted = 1;\n}\nstruct character* get_data(const char* str){\n    if(!sorted)init_data();\n    return (struct character*)bsearch(&str, alphabet, sizeof(alphabet)/sizeof(struct character), sizeof(struct character), m_cmp);\n}\n#endif\n");
	return fclose(signFile) && fclose(inFile);
}
