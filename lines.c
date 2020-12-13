#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#include <time.h>
#define WIDTH 320
#define HEIGHT 240
#define RATIO 4
#define NBUB 300 // bubbles.
#define BUBLIFE 300 // frames.
/*LINE DRAWING DEMO
This program demonstrate's api.h's line rendering capabilities.
The implementation is not the most effective (Berningham's is the most effective AFAIK)
but it does the job.
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
	palette mypal;lSPal(&mypal);
	sprite mysprite;
	sprite mybackspr;
    //char * buffer = 0;
    {
        long length;
        FILE * f = fopen ("backrspr.txt", "r");

        if (f)
        {
          fseek (f, 0, SEEK_END);
          length = ftell (f);
          fseek (f, 0, SEEK_SET);
          backspr = malloc (length);
          if (backspr)
          {
            fread (backspr, 1, length, f);
          }
          fclose (f);
        } else {
        	puts("\nFailed to load Background sprite.\n");
        	exit(1);
        }
    }
	initspfromstr(&mysprite,bubblespr,&mypal);
	initspfromstr(&mybackspr,backspr,&mypal);
	init();//ainit();
	cWin("Line Rendering!",100,100,WIDTH * RATIO,HEIGHT * RATIO,0);
	cRend();cSurf(WIDTH,HEIGHT);srand(time(NULL));
	lSPal(&mypal);
	printf("\nsurf->w = %d",surf->w);
	fflush(stdout);
	for(;!shouldQuit;){
		t+=0.016666;
		resetdbuff(0);
		blitback(&mybackspr,sppos[0],sppos[1]);
		m = 20;
		hshift(wavy);
		m = 40;
		vshift(wavy);
		rensp(&mysprite,sppos[0],sppos[1]);
		for(int i = 0; i < 1000; i++){
			line2d(	rand()%WIDTH, rand()%HEIGHT,
					rand()%WIDTH, rand()%HEIGHT,
					rand()%255,rand()%255,rand()%255);
		}
		ev(EVENT_HANDLER);
		upd();
	}
	freesp(&mybackspr);
	freesp(&mysprite);
	clean();
	return 0;
}
