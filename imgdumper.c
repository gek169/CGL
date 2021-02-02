#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#include "stdlib.h"
#include "stdio.h"
#include <time.h>
/*API_H IMAGE TO RGB PIXEL DATA DUMPER
Dumps raw pixel data from file into 
*/


//	}
//}
int main(int argc, char** argv)
{
	if(argc < 2){
		printf("\nUSAGE:");
		printf("\nimgdumper FILE.png");
	}

	int sw, sh, sc;
	uchar* d = stbi_load(argv[1], &sw, &sh, &sc, 3);
	if(!d){
		printf("\nError opening input file %s",argv[1]);
		return 1;
	}
	FILE* f = NULL; f = fopen("out.rgb", "w");
	if(f){
		for(int i = 0; i < sw * sh * 3; i++)
			fwrite(d+i,1,1,f);
		fclose(f);
		return 0;
	}
	puts("\nERROR opening output file!!!");
	return 1;
}
