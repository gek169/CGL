#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#include "stdlib.h"
#include "stdio.h"
#include <time.h>
/*API_H BINARY FILE VISUALIZER
Visualizes binary files as RGB Pixel Data with 2 offsets:
BYTE OFFSET (A): 0,1,or 2. Basically, is the first value an R, or is the second one R...
TOTAL OFFSET (B): 0 to maximum of an unsigned int. How many sets of three bytes you're shifted into a file, to visualize RGB Pixel data.

Offset A can be changed by pressing space.
*/



unsigned int WIDTH = 640, HEIGHT = 480, SCALE = 2;
unsigned int offsetA, offsetB;


int shouldQuit = 0;
samp* mysamp = NULL;
//void events(SDL_Event* e){
//	switch(e->type){
BEGIN_EVENT_HANDLER
		case SDL_KEYDOWN:
			if(E_KEYSYM == SDLK_f) printf("\nRespects!");
			if(E_KEYSYM == SDLK_SPACE) {offsetA++;offsetA%=3;}
			if(E_KEYSYM == SDLK_RIGHT) {offsetB++;}
			if(E_KEYSYM == SDLK_LEFT) {if(offsetB > 0)offsetB--;}
			if(E_KEYSYM == SDLK_UP) {offsetB+=256;}
			if(E_KEYSYM == SDLK_DOWN) {if(offsetB > 255) offsetB-=256;}
			
		break;
END_EVENT_HANDLER
//	}
//}
int main(int argc, char** argv)
{
	if(argc < 3){
		printf("\nUSAGE:");
		printf("\nbinbrowse FILE -w width -h height -oa offsetA -ob offsetB");
	}
	
	char* filename = argv[1];
	
	char* lastarg = argv[2];
		for(int i = 1; i < argc; i++){
			if(!strcmp("-w",lastarg))
				WIDTH = atoi(argv[i]);
			if(!strcmp("-h",lastarg))
				HEIGHT = atoi(argv[i]);
			if(!strcmp("-oa",lastarg))
				offsetA = atoi(argv[i])%3;
			if(!strcmp("-ob",lastarg))
				offsetB = atoi(argv[i]);
			if(!strcmp("-s",lastarg))
				SCALE = atoi(argv[i]);
			lastarg = argv[i];
		}

	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
	
	char *filecontents = malloc(fsize + 1);
	fread(filecontents, 1, fsize, f);
	fclose(f);
	
	filecontents[fsize] = 0; //Always end in a null terminator!!
	init();
	cWin("C Chads BinBrowser",300,300,WIDTH * SCALE,HEIGHT * SCALE,0);
	cRend();
	cSurf(WIDTH,HEIGHT);srand(time(NULL));
	printf("\nfilename = %s\n",filename);
	for (;shouldQuit < 1;) {
			clear(0);
			for(unsigned int i = 0; i < WIDTH * HEIGHT; i++)
			{
				char* datum = (char*)surf->pixels;
				if(i+ offsetA + offsetB >= fsize) {i = WIDTH * HEIGHT; continue;}
				datum[i] = filecontents[i+ offsetA + offsetB];
			}
			ev(EVENT_HANDLER);
			upd();
	}
	
	clean();

	return EXIT_SUCCESS;
}
