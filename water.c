#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#include <time.h>
/* WATER DEMO
This is a simple heightfield fluid demo written for api.h.
Audio is not used, but mouse movements and clicks are
handled in addition to keyboard events, so this is
probably a superior interface demonstration to
demo.c
*/

#define WIDTH 640
#define HEIGHT 480
int shouldQuit = 0;
unsigned int mousex = 0, mousey = 0;
float du[WIDTH][HEIGHT], dv[WIDTH][HEIGHT], dunew[WIDTH][HEIGHT];

void initFluid(){
	for(int x = 0; x < WIDTH; x++)
	for(int y = 0; y < HEIGHT; y++)
	{
		du[x][y] = fabs(sinf(y/5.0f) + cosf(x/5.0f));
		dv[x][y] = 0.0;
		dunew[x][y] = fabs(sinf(y/5.0f) + cosf(x/5.0f));
	}
}
float get_u(int x, int y){
	if(x<0)x=0;
	if(x>=WIDTH)x=WIDTH-1;
	if(y<0)y=0;
	if(y>=HEIGHT)y=HEIGHT-1;
	return du[x][y];
}
void stepFluid(){
	for(int x = 0; x < WIDTH; x++)
		for(int y = 0; y < HEIGHT; y++)
		{
			dv[x][y] += (get_u(x-1,y) + get_u(x+1, y) + get_u(x,y+1) + get_u(x,y-1))*0.25 - get_u(x,y);
			dv[x][y] *= 0.99;
			dunew[x][y] += dv[x][y];
		}
	for(int x = 0; x < WIDTH; x++)
	for(int y = 0; y < HEIGHT; y++)
	du[x][y] = dunew[x][y];
}
BEGIN_EVENT_HANDLER
		case SDL_KEYDOWN:
			if(E_KEYSYM == SDLK_f) printf("\nRespects!");
			if(E_KEYSYM == SDLK_q) shouldQuit = 1;
		break;
		case SDL_MOUSEMOTION: 
			mousex = E_MOTION.x/2; mousey = E_MOTION.y/2;
		break;
		case SDL_MOUSEBUTTONDOWN:
			if(E_BUTTON==SDL_BUTTON_LEFT) {
				float amt = rand()%100>50?-50:50;
				dunew[MIN(mousex,WIDTH-1)][MIN(mousey,HEIGHT-1)] += amt;
				du[MIN(mousex,WIDTH-1)][MIN(mousey,HEIGHT-1)] += amt;
			}
		break;
END_EVENT_HANDLER
int main()
{
	
	init();//ainit();
	cWin("Wet!",300,300,WIDTH * 2,HEIGHT * 2,0);
	cRend();
	
	cSurf(WIDTH,HEIGHT);srand(time(NULL));
	initFluid();
	for (;shouldQuit < 1;) {
			unsigned int xd, yd;
			xd = rand()%WIDTH;
			yd = rand()%HEIGHT;
			du[xd][yd] += rand()%100 > 50? 15.0 : -15.0;
			stepFluid();
			for(int x = 0; x < WIDTH; x++)
			for(int y = 0; y < HEIGHT; y++)
			{
				char* datum = (char*)surf->pixels;
				float valr = MIN(255,80+du[x][y]*50);
				float valg = MIN(255,80+du[x][y]*50);
				float valb = MIN(255,80+du[x][y]*100);
				datum[4 * (x + y * WIDTH)+R_] = (char)valr;
				datum[4 * (x + y * WIDTH)+G_] = (char)valg;
				datum[4 * (x + y * WIDTH)+B_] = (char)valb;
				datum[4 * (x + y * WIDTH)+A_] = 255;
			}
			ev(EVENT_HANDLER);
			upd();
	}
	//acleanup();
	clean();

	return EXIT_SUCCESS;
}
