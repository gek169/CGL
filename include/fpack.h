#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//Gek's ROMFS format
//Files are RLE
//Starts with a null-terminated filename immediately followed by the size of the file.
int isBigEndian() {
	int a = 1;
	return !((char*)&a)[0];
}

uint32_t endswap32(uint32_t x)
{

    return
    // Source is in format: 0xAARRGGBB
        ((x & 0xFF000000) >> 24) | //______AA
        ((x & 0x00FF0000) >>  8) | //____RR__
        ((x & 0x0000FF00) <<  8) | //__GG____
        ((x & 0x000000FF) << 24);  //BB______
    // Return value is in format:  0xBBGGRRAA
}

void gkpk_fgets(char* buf, uint32_t n, FILE* f){
	char c = fgetc(f);
	for(uint32_t i = 0; c != '\0' && i < n && !feof(f);i++)
		{buf[i] = c;c = fgetc(f);}
}

int gkpk_PackFile(FILE* dest, void* data, uint32_t size, const char* filename){
	if(strlen(filename) > 254 || strlen(filename) < 3) return 1;
	uint32_t size_flipped = endswap32(size);
	fputs(filename, dest); fputc(0, dest);
	if(isBigEndian())
		fwrite(&size, 4, 1, dest);
	else
		fwrite(&size_flipped, 4, 1, dest);
	fwrite(data, 1, size, dest);
	//A couple null bytes to guarantee that files are properly separated.
	fputc(0, dest);fputc(0, dest);
	return 0;
}



FILE* gkpk_GetPackedFile(char* packname, char* filename, uint32_t* filesize){
	FILE* pak = fopen(packname, "r");
	if(pak == NULL) return NULL;
	char cfilename[256];
	uint32_t size;
	gkpk_fgets(cfilename, 256, pak);
	cfilename[255] = '\0';
	while(strcmp(filename, cfilename) && !feof(pak)){
		fread(&size, 4, 1, pak);
		if(feof(pak))break;
		if(!isBigEndian()) size = endswap32(size);
		fseek(pak, size, SEEK_CUR);
		gkpk_fgets(cfilename, 256, pak);
		cfilename[255] = '\0';
	}
	if(strcmp(filename, cfilename) || feof(pak)){
		fclose(pak); return NULL;
	}
	*filesize = size;
	return pak;
}

FILE* gkpk_GiveNextFile(FILE* pak, char* filename, uint32_t* filesize){
	if(feof(pak)){fclose(pak);return 0;}
	char cfilename[256];
	uint32_t size;
	gkpk_fgets(cfilename, 256, pak);
	cfilename[255] = '\0';
	while(strlen(cfilename) < 3 && !feof(pak)){
		gkpk_fgets(cfilename, 256, pak);
		cfilename[255] = '\0';
	}
	if(feof(pak)){fclose(pak);return 0;}
	fread(&size, 4, 1, pak);		
	if(!isBigEndian()) size = endswap32(size);
	*filesize = size;
	strcpy(filename, cfilename);
	printf("\nReturning file with name %s of length %d", filename, size);
	return pak;
}
