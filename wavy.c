#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#include <time.h>
/* WAVY DEMO
This program demonstrats the hshift and vshift routines, which shift
rows or columns of pixels (respectively) by integer amounts.

This allows you to create pretty trippy and very cool effects.
Hshift is much cheaper than vshift, although considering
these effects literally ran on the Am*ga (Albeit with paletted color...)
I don't think your computer is going to struggle.
*/
#define WIDTH 320
#define HEIGHT 240
#define RATIO 4
int shouldQuit = 0;
int sppos[2];
BEGIN_EVENT_HANDLER
	case SDL_KEYDOWN:
		if(E_KEYSYM == SDLK_q) shouldQuit = 1;
	break;
	case SDL_QUIT:shouldQuit = 1;break;
	case SDL_MOUSEMOTION:
		sppos[0] = E_MOTION.x/4;
		sppos[1] = E_MOTION.y/4;
		sppos[0] %= WIDTH;
		sppos[1] %= HEIGHT;
	break;
END_EVENT_HANDLER
const char* backspr = ""
"!%%%!)-!%%%%%!---\n"
"!!%!!)))%%--!----\n"
"!%%%!)-!%%%%%!---\n"
"!---FFFFF%------!\n"
"!%%%!)-!%%%%%!-!!\n"
"!!%!!)))%%--%-!!!";
const char* bubblespr = "\n"
"       `````   \n"
"    ```      ``\n"
"   ``    KK   `\n"
"   `    K K   ``\n"
"   `     K    `\n"
"   ``        ``\n"
"    ``     ```\n"
"      `````";//Notice how the last line does not end in \n
float t = 0.0;
float m = 10;
int wavy(int i){
	return m * sinf(t + (float)i/10);
}
int main()
{
	palette mypal;lSPal(&mypal);
	sprite mysprite;
	sprite mybackspr;
	initspfromstr(&mysprite,bubblespr,&mypal);
	initspfromstr(&mybackspr,backspr,&mypal);
	init();//ainit();
	cWin("Sprite Rendering!",300,300,WIDTH * RATIO,HEIGHT * RATIO,0);
	cRend();cSurf(WIDTH,HEIGHT);srand(time(NULL));
	lSPal(&mypal);
	printf("\nsurf->w = %d",surf->w);
	fflush(stdout);
	for(;!shouldQuit;){
		t += 16.66666/1000;
		blitback(&mybackspr,sppos[0],sppos[1]);
		m = 20;
		hshift(wavy);
		m = 40;
		vshift(wavy);
		rensp(&mysprite,sppos[0],sppos[1]);
		ev(EVENT_HANDLER);
		upd();
		if(t>1000)t-=1000;
	}
	freesp(&mybackspr);
	freesp(&mysprite);
	clean();
	return 0;
}