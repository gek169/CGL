#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#include <time.h>
#define WIDTH 320
#define HEIGHT 240
#define RATIO 4
#define NBUB 300 // bubbles.
#define BUBLIFE 300 // frames.
/*TILEMAP RENDERING DEMO
This program demonstrates rendering a series of sprites in a world coordinate system
*/
int shouldQuit = 0;
int sppos[2];
typedef struct{
	int life;
	float x,y,vx, vy;
} bubble;
bubble bubbles[NBUB];
float randf(){return (float)rand()/(float)RAND_MAX;}

char* backspr = NULL; // to be loaded from file.
const char* bubblespr = "\n"
"    ``````  \n"
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

int camx, camy;
int arrowkeys[4]; //UDLR
typedef struct{
	sprite* s;
	int xp, yp;
} entity;
#define MAX_ENTITIES 2048
entity entities[MAX_ENTITIES];//You can have a lot of entities!
void initents(){
	for(size_t i = 0; i < MAX_ENTITIES; i++){
		entities[i].s = 0;
		entities[i].xp = 0;
		entities[i].yp = 0;
	}
}
void renents(int cx, int cy){
	for(size_t i = 0; i < MAX_ENTITIES; i++){
		if(!entities[i].s)continue;
		rensp(entities[i].s,entities[i].xp - cx, entities[i].yp - cy);
	}
}

BEGIN_EVENT_HANDLER
	case SDL_KEYUP:
		if(E_KEYSYM == SDLK_q) shouldQuit = 1;
		if(E_KEYSYM == SDLK_UP) arrowkeys[0] = 0;
		if(E_KEYSYM == SDLK_DOWN) arrowkeys[1] = 0;
		if(E_KEYSYM == SDLK_LEFT) arrowkeys[2] = 0;
		if(E_KEYSYM == SDLK_RIGHT) arrowkeys[3] = 0;
	break;
	case SDL_KEYDOWN:
		if(E_KEYSYM == SDLK_UP) arrowkeys[0]   = 2;
		if(E_KEYSYM == SDLK_DOWN) arrowkeys[1] = 2;
		if(E_KEYSYM == SDLK_LEFT) arrowkeys[2] = 2;
		if(E_KEYSYM == SDLK_RIGHT) arrowkeys[3]= 2;
	break;
	case SDL_QUIT:shouldQuit = 1;break;
	case SDL_MOUSEMOTION:
		sppos[0] = E_MOTION.x/RATIO;
		sppos[1] = E_MOTION.y/RATIO;
		sppos[0] %= WIDTH;
		sppos[1] %= HEIGHT;
	break;
END_EVENT_HANDLER


int main()
{
	palette mypal;lSPal(&mypal);
	sprite mysprite; //bubble sprite
	sprite mybackspr; //wibbly wobbly background
	sprite floor;
	sprite wall;
	lspr(&floor,"floor1.png");
	lspr(&wall,"wall1.png");
	//this fixes a bug in SDL2 so that the controls start responding immediately after you click in the window.
	tilemap World;
	initTileMap(&World,500,500,16,16);
	initents();
	srand(time(NULL));
	for(uint i = 0; i < World.w * World.h; i++){
		*(World.d + i) = &floor;
		if(rand()%1000 < 50)*(World.d + i) = NULL;
		if(rand()%1000 < 100||
			i%500 == 0 ||
			i%500 == World.w-1 ||
			i/500 == 0 ||
			i/500 == World.h-1)
			*(World.d + i) = &wall;
	}
	camx = - WIDTH; camy = - HEIGHT;
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
	cRend();cSurf(WIDTH,HEIGHT);
	lSPal(&mypal);
	fflush(stdout);
	for(int i = 0; i < NBUB; i++) bubbles[i].life = BUBLIFE + 10;
	for(int i = 0; i < 1000 && i < MAX_ENTITIES; i++){
		entities[i].s = &mysprite;
		entities[i].xp = rand()%(World.w * World.sprw-World.sprw);
		entities[i].yp = rand()%(World.h * World.sprh-World.sprh);
	}
	arrowkeys[0]=0;
	arrowkeys[1]=0;
	arrowkeys[2]=0;
	arrowkeys[3]=0;
	for(;!shouldQuit;){
		t+=0.016666;
		camx -= arrowkeys[2] - arrowkeys[3];
		camy += arrowkeys[1] - arrowkeys[0];
		if(camx < WIDTH)camx = WIDTH;
		if(camy < HEIGHT)camy = HEIGHT;
		if(camx > (int)World.w*(int)World.sprw - (int)WIDTH)camx = (int)World.w*(int)World.sprw - (int)WIDTH;
		if(camy > (int)World.h*(int)World.sprh - (int)HEIGHT)camy= (int)World.h*(int)World.sprh - (int)HEIGHT;
		resetdbuff(0);
		blitback(&mybackspr,sppos[0],sppos[1]);
		for(int i = 0; i < NBUB; i++){
			if(bubbles[i].life < BUBLIFE && bubbles[i].y > -mysprite.h){
				bubbles[i].vx += (randf()-0.5)*0.2;
				bubbles[i].vy += (randf()-0.6)*0.2;
				bubbles[i].x += bubbles[i].vx;
				bubbles[i].y += bubbles[i].vy;
				//renspdbuff(&mysprite,bubbles[i].x,bubbles[i].y,-0.3+randf());
				rensp(&mysprite,bubbles[i].x,bubbles[i].y);
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
		m = 6;
		hshift(wavy);
		m = 6;
		vshift(wavy);
		//Now we render our tilemap.
		World.xoff = camx;
		World.yoff = camy;
		renTileMap(&World);
		renents(camx, camy);
		ev(EVENT_HANDLER);
		upd();
		printf("\nSTATES: %d, %d, %d, %d", arrowkeys[0],
										arrowkeys[1],
										arrowkeys[2],
										arrowkeys[3]);
	}
	freesp(&mybackspr);
	freesp(&mysprite);
	freesp(&floor);
	freesp(&wall);
	freeTileMap(&World);
	clean();
	return 0;
}
