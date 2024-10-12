#include <stdio.h>
#include <stdlib.h>

#define TBUF 1

int main(int argc, char ** argv){
	FILE *fddst = fopen(argv[2], "wb");
	FILE *fdsrc = fopen(argv[1], "rb");
	char BUFFER[TBUF];
	int nbread;
	while((nbread = fread(BUFFER, sizeof(char), TBUF, fdsrc)) > 0){
		fwrite(BUFFER, sizeof(char), nbread, fddst);
	}
	fclose(fddst);
	fclose(fdsrc);
}
