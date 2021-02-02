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



unsigned int WIDTH = 640, HEIGHT = 480, SCALE = 2, STEPSIZE = 1024;
unsigned char CLEARCOLOR = (unsigned char)0;
unsigned int offsetA = 0, offsetB = 0;


int shouldQuit = 0;
samp* mysamp = NULL;
//void events(SDL_Event* e){
//	switch(e->type){
BEGIN_EVENT_HANDLER
		case SDL_KEYDOWN:
			if(E_KEYSYM == SDLK_f) printf("\nRespects!");
			if(E_KEYSYM == SDLK_SPACE) {offsetA++;offsetA%=3;}
			if(E_KEYSYM == SDLK_RIGHT) {offsetB++;printf("\noffsetB = %d",offsetB);}
			if(E_KEYSYM == SDLK_LEFT) {if(offsetB > 0)offsetB--;printf("\noffsetB = %d",offsetB);}
			if(E_KEYSYM == SDLK_UP) {offsetB+=STEPSIZE;printf("\noffsetB = %d",offsetB);}
			if(E_KEYSYM == SDLK_DOWN) {if(offsetB >= STEPSIZE) offsetB-=STEPSIZE;printf("\noffsetB = %d",offsetB);}
			
		break;
END_EVENT_HANDLER
//	}
//}
int main(int argc, char** argv)
{
	if(argc < 3){
		printf("\nUSAGE:");
		printf("\nbinbrowse FILE -w width -h height -oa offsetA -ob offsetB -s scale -c clearshade");
	}
	
	char* filename = argv[1];
	
	char* lastarg = argv[2];
		for(int i = 1; i < argc && argc > 2; i++){
			if(!strcmp("-w",lastarg))
				WIDTH = atoi(argv[i]);
			if(!strcmp("-h",lastarg))
				HEIGHT = atoi(argv[i]);
			if(!strcmp("-oa",lastarg))
				offsetA = atoi(argv[i])%3;
			if(!strcmp("-ob",lastarg))
				offsetB = atoi(argv[i]);
			if(!strcmp("-c",lastarg))
				CLEARCOLOR = (unsigned char)atoi(argv[i]);
			if(!strcmp("-step",lastarg))
				STEPSIZE = atoi(argv[i]);
			if(!strcmp("-s",lastarg))
				SCALE = atoi(argv[i]);
			
			lastarg = argv[i];
		}

	printf("\nAttempting to open filename = %s\n",filename);

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
	printf("\nOpened filename = %s\n",filename);
	for (;shouldQuit < 1;) {
			clear(CLEARCOLOR);
			if(offsetB*3 > fsize){offsetB = (fsize- 1)/3;}
			for(unsigned int i = 0; i < WIDTH * HEIGHT; i++)
			{
				unsigned int* datum = (unsigned int*)surf->pixels;
				uint index = (i + offsetB)*3 + offsetA;
				uchar* pixpoint = (unsigned char*)(datum + i);
				if(index + 2 >= fsize) break;
				pixpoint[0] = filecontents[index+0];
				pixpoint[1] = filecontents[index+1];
				pixpoint[2] = filecontents[index+2];
				//datum[i] = pixel;
			}
			ev(EVENT_HANDLER);
			upd();
	}
	
	clean();

	return EXIT_SUCCESS;
}
