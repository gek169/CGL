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
struct{
	vec3 p;
	float ang;
	float horizon;
	float scale_height;
	float distance;
} camera;
sprite heighttex;
sprite colortex;
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

void DrawVerticalLine(unsigned int x, unsigned int height, unsigned int low, unsigned int col){
	if(x >= WIDTH || height >= HEIGHT || low >= HEIGHT || low <= height) return;
	x %= WIDTH;
	//low = HEIGHT-1;
	//printf("\nGot color");
	for(unsigned int i = low; i >= height && i < HEIGHT; i--){
		unsigned int* datum = (unsigned int*)surf->pixels;
		//printf("\nwriting to %d, %d", x,i);
		datum[x + i * WIDTH] = col;
	}
	//printf("\noh shit it actually worked");
}

ivec3 ftos(ivec3 samppoint){
	while (samppoint.d[0] < 0) samppoint.d[0] += heighttex.w;
	while (samppoint.d[1] < 0) samppoint.d[1] += heighttex.h;
	samppoint.d[0] %= heighttex.w;
	samppoint.d[1] %= heighttex.h;
	return samppoint;
}

unsigned int sampleheight(ivec3 s){
	return 0;
	if(s.d[0] < 0 || s.d[0] > 1024 ||
	   s.d[1] < 0 || s.d[1] > 1024)
	   return 0;
	s = ftos(s);
	uchar result = ((uchar*)heighttex.d)[
   		(
   	 		(s.d[0]) + 
   	 		(s.d[1]) * WIDTH
   	 	)*4
   	];
   	return (unsigned int)result;
}

unsigned int samplecolor(ivec3 s){
	s = ftos(s);
	unsigned int result =((unsigned int*)colortex.d)
	[s.d[0] + s.d[1] * colortex.w];
	return result;
}

void Render(){
	vec3 p; float phi; float height; float horizon; float scale_height; float distance;
	//For this demo.
	p = camera.p;
	phi = camera.ang;
	height = camera.p.d[2];
	horizon = camera.horizon;
	scale_height = camera.scale_height;
	distance = camera.distance;
    //# precalculate viewing angle parameters
    float sinphi = sinf(phi);
    float cosphi = cosf(phi);
    
    //# initialize visibility array. Y position for each column on screen 
    float ybuffer[WIDTH];
    //for i in range(0, screen_width):
    for(int i = 0; i < WIDTH; i++)
        ybuffer[i] = HEIGHT;

    //# Draw from front to the back (low z coordinate to high z coordinate)
    float dz = 1.0;
    float z = 1.0;
    while(z < distance){
        //# Find line on map. This calculation corresponds to a field of view of 90°
        vec3 pleft = {
            .d[0] = (-cosphi*z - sinphi*z) + p.d[0],
            
            .d[1] = (sinphi*z - cosphi*z) + p.d[1],
            .d[2] = 0
            };
        vec3 pright = {
            .d[0] = ( cosphi*z - sinphi*z) + p.d[0],
            .d[1] = (-sinphi*z - cosphi*z) + p.d[1],
            .d[2] = 0
        };

        //# segment the line
        float dx = (pright.d[0] - pleft.d[0]) / WIDTH;
        float dy = (pright.d[1] - pleft.d[1]) / WIDTH;

        //# Raster line and draw a vertical line for each segment
        //for i in range(0, screen_width):
        for(int i = 0; i < WIDTH; i++){
        	ivec3 plefti;
        	plefti.d[0] = pleft.d[0];
        	plefti.d[1] = pleft.d[1];
			int hsamp = sampleheight(plefti);
            int height_on_screen = (
            	height - hsamp
            )/z * scale_height + horizon;
            unsigned int col = samplecolor(plefti);
			//printf("\nBefore line render, x = %d, z = %f", i, z);
            DrawVerticalLine(i, height_on_screen, ybuffer[i], col);
			//printf("\nAfter line render, x = %d, z = %f", i, z);
            if(height_on_screen < ybuffer[i])
                ybuffer[i] = height_on_screen;
            pleft.d[0] += dx;
            pleft.d[1] += dy;
		}
        //# Go to next line and increase step size when you are far away
        z += dz;
        //dz += 0.2;
    }
}
/*
void  Render()
{
	const float distfactor = 0.2
	vec3 p; float phi; float height; float horizon; float scale_height; float distance;
	//For this demo.
	p = camera.p;
	phi = camera.ang;
	height = camera.p.d[2];
	horizon = camera.horizon;
	scale_height = camera.scale_height;
	distance = camera.distance;
    //# precalculate viewing angle parameters
    float sinphi = sinf(phi);
    float cosphi = cosf(phi);
    
	for(float z = distance; z > 1.0f; z-=0.2){
		float width_mult = z * distfactor;
		float x_offset = width_mult * -1.0;
		float x_step = 2.0 * width_mult / (float)WIDTH;
		for(int i = 0; i < WIDTH; i++, x_offset += x_step){
			
		}
	}
}
*/
int main()
{
	srand(time(NULL));
	camera.p = (vec3){.d[0] = 1024,.d[1] = -100,.d[2] = 200};
	camera. ang = 0; 
	camera. horizon = HEIGHT/2;
	camera. scale_height = 50;
	camera. distance = 500;
	
	palette mypal;lSPal(&mypal);
	sprite mysprite;
	sprite mybackspr;
	
	initspfromstr(&mysprite,bubblespr,&mypal);
	initspfromstr(&mybackspr,backspr,&mypal);
	lspr(&heighttex, "D5W.png");
	lspr(&colortex, "C5W.png");
	printf("\nLOADED THE SHIT!!!\n");fflush(stdout);
	init();//ainit();
	cWin("Sprite Rendering!",300,300,WIDTH * RATIO,HEIGHT * RATIO,0);
	cRend();cSurf(WIDTH,HEIGHT);srand(time(NULL));
	lSPal(&mypal);
	printf("\nsurf->w = %d",surf->w);
	fflush(stdout);
	int ox = 0;
	for(;!shouldQuit;){
		ox+=2;
		//camera.ang+=0.01;
		if(camera.ang > 2 * 3.14159)
			camera.ang -= 2*3.14159;
		camera.p.d[0]+= 2;
		if(camera.p.d[0] > 1024)
			camera.p.d[0]-=1024;
		if(ox > 1024) ox -= 1024;
		t += 16.66666/1000;
		blitback(&mybackspr,sppos[0],sppos[1]);
		blitback(&colortex,ox,0);
		m = 20;
		//hshift(wavy);
		m = 40;
		//vshift(wavy);
		//TODO Render terrain.
		Render();
		
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
