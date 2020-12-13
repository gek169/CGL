#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#include <time.h>
/*API_H INTERFACE DEMO
this is the first demo written to demonstrate api.h's wrapper
as a convenient replacement for hand-writing SDL2 code.

api.h was written with the original intent to be a C golfing target,
I.E. we all agree to use api.h when golfing C code. It's basically a bunch
of boilerplate rendering/event handling code anyway.

The gpu only acts to write the screen buffer to the screen,
not to do any of the program rendering. Program rendering is done
on the CPU in api.h.
*/







int shouldQuit = 0;
samp* mysamp = NULL;
//void events(SDL_Event* e){
//	switch(e->type){
BEGIN_EVENT_HANDLER
		case SDL_KEYDOWN:
			if(E_KEYSYM == SDLK_f) printf("\nRespects!");
			if(E_KEYSYM == SDLK_q) shouldQuit = 1;
			if(E_KEYSYM == SDLK_g) aplay(mysamp,0);
		break;
END_EVENT_HANDLER
//	}
//}
int main()
{
	
	init();ainit();
	cWin("Cat",300,300,640,480,0);
	cRend();
	mysamp = lwav("Zube.wav");
	cSurf(640,480);srand(time(NULL));
	int depth = surf->pitch/surf->w;
	printf("\ndepth = %d\n",depth);
	for (;shouldQuit < 1;) {
			for(int x = 0; x < surf->w; x++)
			for(int y = 0; y < surf->h; y++)
			{
				char* datum = (char*)surf->pixels;
				datum += depth * (x + y * surf->w);
				datum[R_] += rand()%100 > 50? rand()%3 : -1 * rand()%3;
				//datum[1] += rand()%100 > 50? rand()%3 : -1 * rand()%3;
				//datum[2] += rand()%100 > 50? rand()%3 : -1 * rand()%3;
				datum[G_] = 0;
				datum[B_] = 0;
				datum[A_] = 255;
			}
			ev(EVENT_HANDLER);
			upd();
	}
	Mix_FreeChunk(mysamp);
	acleanup();
	clean();

	return EXIT_SUCCESS;
}
