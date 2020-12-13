#define CHAD_API_IMPL
//#define USE_MIXER
#include "include/api.h"
#include <time.h>
#define WIDTH 200
#define HEIGHT 160
#define RATIO 6
/*Triangle Rendering Demo
Demonstrates the speed of api.h's triangle routine.
No, it's not that fast.
*/
int shouldQuit = 0;
int sppos[2];
float randf(){return (float)rand()/(float)RAND_MAX;}
BEGIN_EVENT_HANDLER
	case SDL_KEYDOWN:
		if(E_KEYSYM == SDLK_q) shouldQuit = 1;
	break;
	case SDL_QUIT:shouldQuit = 1;break;
	case SDL_MOUSEMOTION:
		sppos[0] = E_MOTION.x/RATIO;
		sppos[1] = E_MOTION.y/RATIO;
		sppos[0] %= WIDTH;
		sppos[1] %= HEIGHT;
	break;
END_EVENT_HANDLER
char* backspr = NULL; // to be loaded from file.
const char* bubblespr = "\n"
"    `````   \n"
" ```      ``\n"
"``    KK   `\n"
"`    K K   ``\n"
"`     K    `\n"
"``        ``\n"
" ``     ```\n"
"   `````";//Notice how the last line does not end in \n

float t = 0.0;
float m = 10;
int wavy(int i){
	return m * sinf(t + (float)i/10);
}

int main()
{
	initFont("GKBAF16x16.bmp");
	init();//ainit();
	cWin("Triangle Rendering!",100,100,WIDTH * RATIO,HEIGHT * RATIO,0);
	cRend();cSurf(WIDTH,HEIGHT);srand(time(NULL));
	printf("\nsurf->w = %d",surf->w);
	fflush(stdout);
	for(;!shouldQuit;){
		
		
		for(int i = 0; i < 9000; i++){
			tri2dND(rand()%WIDTH, rand()%HEIGHT,// (randf()-0.5)*3,
					rand()%WIDTH, rand()%HEIGHT,// (randf()-0.5)*3,
					rand()%WIDTH, rand()%HEIGHT,// (randf()-0.5)*3,
					rand()%255,rand()%255,rand()%255);
		}
		rensp(charsprites + (uchar)'A',sppos[0],sppos[1]);
		
		drawText(0,HEIGHT-charsprites['!'].h*4,
		"Written by\nthe C Chads\nSept 2020!",
		128,255,255
		);
		ev(EVENT_HANDLER);
		upd();
	}
	clean();
	return 0;
}
