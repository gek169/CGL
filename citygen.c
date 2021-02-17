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
#define MAX_ROAD_NODES 1024
#define ROAD_NETWORK_WIDTH 32
#define NEIGHBOR_UP 0
#define NEIGHBOR_DOWN 1
#define NEIGHBOR_LEFT 2
#define NEIGHBOR_RIGHT 3
typedef struct {
	uint neighbors[4]; //UDLR
	f_ x, y, elev;
} RoadNode;
typedef struct {
	RoadNode nodes[MAX_ROAD_NODES];
	f_ xoff, yoff, minelev;
	f_ xdim, ydim, zdim;
} RoadNetwork;
RoadNetwork myNetwork;
void initNode(RoadNode* node){
	node->x = 0;
	node->y = 0;
	node->elev = 0;
	node->neighbors[0]=MAX_ROAD_NODES+1;
	node->neighbors[1]=MAX_ROAD_NODES+1;
	node->neighbors[2]=MAX_ROAD_NODES+1;
	node->neighbors[3]=MAX_ROAD_NODES+1;
}
void initNetwork(RoadNetwork* network, f_ xoff, f_ yoff, f_ minelev, f_ xdim, f_ ydim, f_ zdim){
	for(int i = 0; i < MAX_ROAD_NODES; i++){
		initNode(network->nodes+i);
		RoadNode* node = network->nodes+i;
		if(i%ROAD_NETWORK_WIDTH != 0)
			node->neighbors[NEIGHBOR_LEFT] = i-1;
		if(i%ROAD_NETWORK_WIDTH != ROAD_NETWORK_WIDTH-1)
			node->neighbors[NEIGHBOR_RIGHT] = i+1;
		if(MAX_ROAD_NODES-ROAD_NETWORK_WIDTH > i)
			node->neighbors[NEIGHBOR_DOWN] = i+ROAD_NETWORK_WIDTH;
		if(i>=ROAD_NETWORK_WIDTH)
			node->neighbors[NEIGHBOR_UP] = i-ROAD_NETWORK_WIDTH;
	}
}

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
	initNetwork(&myNetwork, 0, 0, 0, 64, 64, 50);
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
		/*for(int i = 0; i < 1000; i++){
			line2d(	rand()%WIDTH, rand()%HEIGHT,
					rand()%WIDTH, rand()%HEIGHT,
					rand()%255,rand()%255,rand()%255);
		}*/
		ev(EVENT_HANDLER);
		upd();
	}
	freesp(&mybackspr);
	freesp(&mysprite);
	clean();
	return 0;
}
