#define CHAD_API_IMPL
//#define USE_MIXER
#include "include/api.h"
#define CHAD_PHYS_IMPL
#include "include/chadphys.h"
#include <time.h>
#define WIDTH 320
#define HEIGHT 240
#define RATIO 3
/*3dMath.h collision detection testing program
tests the functionality of 3dMath.h's collision detection routines.

*/
int shouldQuit = 0;
int sppos[2];
uchar mode = 0; //0 for the box test, 1 for the sphere test.
float randf(){return (float)rand()/(float)RAND_MAX;}
BEGIN_EVENT_HANDLER
	case SDL_KEYDOWN:
		if(E_KEYSYM == SDLK_q) shouldQuit = 1;
	break;
	case SDL_QUIT:shouldQuit = 1;break;
	case SDL_MOUSEBUTTONDOWN:
		if(E_BUTTON==SDL_BUTTON_LEFT) {
			mode++;
			mode%=3;
		}
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


aabb obj1;
aabb obj2;

void renderAABB(aabb obj){
	//top side
	line2d(	obj.c.d[0] - obj.e.d[0], obj.c.d[1] + obj.e.d[1],
			obj.c.d[0] + obj.e.d[0], obj.c.d[1] + obj.e.d[1],
			255,255,255);
	//bottom side
	line2d(	obj.c.d[0] - obj.e.d[0], obj.c.d[1] - obj.e.d[1],
			obj.c.d[0] + obj.e.d[0], obj.c.d[1] - obj.e.d[1],
			255,255,255);
	//left side
	line2d(	obj.c.d[0] - obj.e.d[0], obj.c.d[1] - obj.e.d[1],
			obj.c.d[0] - obj.e.d[0], obj.c.d[1] + obj.e.d[1],
			255,255,255);
	//Right side
	line2d(	obj.c.d[0] + obj.e.d[0], obj.c.d[1] - obj.e.d[1],
			obj.c.d[0] + obj.e.d[0], obj.c.d[1] + obj.e.d[1],
			255,255,255);
}
void renderCircle(aabb obj){
	const float PI = 3.14159;
	const float quality = 30;
	for(int i = 0; i < quality+1; i++){
		float px = obj.c.d[3] * sinf(2*PI/quality * (float)i) + obj.c.d[0];
		float pxp = obj.c.d[3] * sinf(2*PI/quality * (float)(i-1)) + obj.c.d[0];
		float py = obj.c.d[3] * cosf(2*PI/quality * (float)i) + obj.c.d[1];
		float pyp = obj.c.d[3] * cosf(2*PI/quality * (float)(i-1)) + obj.c.d[1];
		line2d(px, py, pxp, pyp, 255, 255, 255);
	}
}
int main()
{
	initFont("GKBAF16x16.bmp");
	init();//ainit();
	obj1.c.d[0] = 80;
	obj1.c.d[1] = 80;
	obj1.c.d[2] = 0;
	obj1.c.d[3] = 40; //radius in the sphere test
	
	obj1.e.d[0] = 10;
	obj1.e.d[1] = 10;
	obj1.e.d[2] = 300;

	obj2.c.d[0] = 20;
	obj2.c.d[1] = 20;
	obj2.c.d[2] = 0;
	obj2.c.d[3] = 40; //radius in the sphere test
	
	obj2.e.d[0] = 10;
	obj2.e.d[1] = 15;
	obj2.e.d[2] = 300;
	cWin("Triangle Rendering!",100,100,WIDTH * RATIO,HEIGHT * RATIO,0);
	cRend();cSurf(WIDTH,HEIGHT);srand(time(NULL));
	printf("\nsurf->w = %d",surf->w);
	fflush(stdout);
	for(;!shouldQuit;){
		
		clear(0);
		//rensp(charsprites + (uchar)'A',sppos[0],sppos[1]);
		obj2.c.d[0] = sppos[0];
		obj2.c.d[1] = sppos[1];
		if(mode == 0){
			renderAABB(obj1);
			renderAABB(obj2);
			vec4 ret = boxvbox(obj1, obj2);
			if(ret.d[3] <= 0)
				drawText(0,HEIGHT-charsprites['!'].h*4,
				"Intersecting: False",
				255,50,20
				);
			else{
				drawText(0,HEIGHT-charsprites['!'].h*4,
					"Intersecting: True!",
					0,255,0
					);
				line2d(
					WIDTH/2, 			HEIGHT/2,
					WIDTH/2 + ret.d[0], HEIGHT/2 + ret.d[1],
					255,255,0
				);
				//Render object 2 pushed back.
				aabb obj2pushed = obj2;
				obj2pushed.c.d[0] += ret.d[0];
				obj2pushed.c.d[1] += ret.d[1];
				renderAABB(obj2pushed);
			}
		}
		if(mode == 1){
			renderCircle(obj1);
			renderCircle(obj2);
			vec4 ret = spherevsphere(obj1.c, obj2.c);
			if(ret.d[3] <= 0)
				drawText(0,HEIGHT-charsprites['!'].h*4,
				"Intersecting: False",
				255,50,20
				);
			else {
				drawText(0,HEIGHT-charsprites['!'].h*4,
					"Intersecting: True!",
					0,255,0
					);
				line2d(
					WIDTH/2, 			HEIGHT/2,
					WIDTH/2 + ret.d[0], HEIGHT/2 + ret.d[1],
					255,255,0
				);
				aabb obj2pushed = obj2;
				obj2pushed.c.d[0] += ret.d[0];
				obj2pushed.c.d[1] += ret.d[1];
				renderCircle(obj2pushed);
			}
		}
		if(mode==2){
			renderAABB(obj2);
			renderCircle(obj1);
			vec4 ret = spherevaabb(obj1.c, obj2);
			vec3 closest = closestpointAABB(obj2, downv4(obj1.c));
			line2d(
				closest.d[0], closest.d[1],
				obj2.c.d[0], obj2.c.d[1],
				0,255,0
			);
			if(ret.d[3] <= 0)
				drawText(0,HEIGHT-charsprites['!'].h*4,
				"Intersecting: False",
				255,50,20
				);
			else {
				drawText(0,HEIGHT-charsprites['!'].h*4,
					"Intersecting: True!",
					0,255,0
					);
				line2d(
					WIDTH/2, 			HEIGHT/2,
					WIDTH/2 + ret.d[0], HEIGHT/2 + ret.d[1],
					255,255,0
				);
				//Render object 2 pushed back.
				aabb obj2pushed = obj2;
				obj2pushed.c.d[0] += ret.d[0];
				obj2pushed.c.d[1] += ret.d[1];
				renderAABB(obj2pushed);
			}
		}
		ev(EVENT_HANDLER);
		upd();
	}
	clean();
	return 0;
}
