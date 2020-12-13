#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#include <time.h>
#define WIDTH 320
#define HEIGHT 240
#define RATIO 4
#define NBUB 300 // bubbles.
#define BUBLIFE 300 // frames.
/*SPRITE RENDERING DEMO
This program demonstrate's api.h's sprite rendering capabilities.

Sprites are stored as raw image data and indeed you can create
a sprite from image data by manually copying data in (RGBA)

However api.h also allows you to define palettes- Mappings from
the ascii character set to R,G,B values, so that you can create
sprites in text files (Or in code! Both are demonstrated)

On my i7-6700 i'm able to render upwards of 30,000 sprites along with
the hshift routine with no dips in framerate (CPU would be the bottleneck)

You can probably tell, I really like bubbles :)
*/
int shouldQuit = 0;
int sppos[2];
typedef struct{
	int life;
	float x,y,vx, vy;
} bubble;
bubble bubbles[NBUB];
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
"    ``````  \n"
" ```      ``\n"
"``    KK   `\n"
"`    K K   ``\n"
"`     K    `\n"
"``   K    ``\n"
" ``  K  ```\n"
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
	cWin("Sprite Rendering!",300,300,WIDTH * RATIO,HEIGHT * RATIO,0);
	cRend();cSurf(WIDTH,HEIGHT);srand(time(NULL));
	lSPal(&mypal);
	fflush(stdout);
	for(int i = 0; i < NBUB; i++) bubbles[i].life = BUBLIFE + 10;
	for(;!shouldQuit;){
		t+=0.016666;
		resetdbuff(0);
		blitback(&mybackspr,sppos[0],sppos[1]);
		//rensp(&mysprite,sppos[0],sppos[1]);
		m = 20;
		hshift(wavy);
		//m = 40;
		//vshift(wavy);
		for(int i = 0; i < NBUB; i++){
			if(bubbles[i].life < BUBLIFE && bubbles[i].y > -mysprite.h){
				bubbles[i].vx += (randf()-0.5)*0.2;
				bubbles[i].vy += (randf()-0.6)*0.2;
				bubbles[i].x += bubbles[i].vx;
				bubbles[i].y += bubbles[i].vy;
				//renspdbuff(&mysprite,bubbles[i].x,bubbles[i].y,-0.3+randf());
				renspadv(&mysprite,bubbles[i].x,bubbles[i].y,3);
				bubbles[i].life++;
			}
			else{
				bubbles[i].x = randf()*WIDTH*1.2-(WIDTH/10.0);
				bubbles[i].y = randf()*0.2*HEIGHT+HEIGHT;
				bubbles[i].vx = (randf()-0.5)*3;
				bubbles[i].vy = (randf()-1.3)*2;
				bubbles[i].life = rand()%100;
			}
		}
		ev(EVENT_HANDLER);
		
		upd();
	}
	freesp(&mybackspr);
	freesp(&mysprite);
	clean();
	return 0;
}
