#define CHAD_API_IMPL
#define USE_MIXER
#include "include/api.h"
#define STB_PERLIN_IMPLEMENTATION
#include "include/stb_perlin.h"
#include <time.h>
/* DYNAMIC HEIGHTFIELD RENDERING

Generates a heightfield based on a perlin noise, and a seed provided by commandline arguments.
*/
#define WIDTH 320
#define HEIGHT 240
#define hmapw 2048
#define hmaph 2048
//0 east (+X)
//1 west (-X)
//2 south (+Y)
//3 north (-Y)
#define SUNDIR 0
//^ We are marching EASTWARD to look for peaks.
#define SUN_CLIMB_COEFF 0.3f
//^ We are looking for objects rising above this line from our height, with this line rising EASTWARD.
#define RATIO 3
#define seed 0
#define nsamps 2
const float sampletable[4] = {1.0/256.0, 1.5,
						1.0/64.0, 1.0/8.0};
const int samplewraps[2] = {8, 32};
//number of colors in the color table
#define ncolors 5
#define shadowshift 2
const uchar colortable[3*ncolors] = {
	0x23, 0xC2, 0xDB, //water
	0xFF, 0xEE, 0x99, //sand
	0x00, 0xB3, 0x00, //grass
	0x7A, 0x7A, 0x7A, //Rock
	0xD1, 0xD1, 0xD1, //Mountaintops
};
const uchar transitionpoints[ncolors-1] = {
	20, //Water ->sand
	60, //sand  ->grass
	120, //grass->rock
	200, //rock ->Mountaintops
};
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
		sppos[0] = E_MOTION.x/RATIO;
		sppos[1] = E_MOTION.y/RATIO;
		sppos[0] %= WIDTH;
		sppos[1] %= HEIGHT;
	break;
END_EVENT_HANDLER
const char* backspr = ""
"88888888888888888\n"
"88888888888888888\n"
"88888888888888888\n"
"88888888888888888\n"
"88888888888888888\n"
"88888888888888888";
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
	/*
	if(s.d[0] < 0 || s.d[0] > 1024 ||
	   s.d[1] < 0 || s.d[1] > 1024)
	   return 0;
	*/
	s = ftos(s);
	uchar result = ((uchar*)heighttex.d)[
   		(
   	 		(s.d[0]) + 
   	 		(s.d[1]) * heighttex.w
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
        ybuffer[i] = HEIGHT-1;

    //# Draw from front to the back (low z coordinate to high z coordinate)
    float dz = 1.0;
    float z = 0.1;
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
        dz += 0.05;
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
	camera.p = (vec3){.d[0] = 1024,.d[1] = -100,.d[2] = 230};
	camera. ang = 3.14159 * 0.5; 
	camera. horizon = HEIGHT/2;
	camera. scale_height = 80;
	camera. distance = 2000;
	
	palette mypal;lSPal(&mypal);
	sprite mysprite;
	sprite mybackspr;
	
	initspfromstr(&mysprite,bubblespr,&mypal);
	initspfromstr(&mybackspr,backspr,&mypal);
	//lspr(&heighttex, "D5W.png");
	colortex.w = hmapw;
	colortex.h = hmaph;
	colortex.d = malloc(hmapw * hmaph * 4);
	heighttex.w = hmapw;
	heighttex.h = hmaph;
	heighttex.d = malloc(hmapw * hmaph * 4);
	//Fill in the height map.
	for(int i = 0; i < heighttex.w; i++)
	for(int j = 0; j < heighttex.h; j++){
		float val = 0;
		for(int k = 0; k < nsamps; k++)
			val += sampletable[2*k+1] * stb_perlin_noise3_seed(
				i*sampletable[2*k], j*sampletable[2*k], 0, 
				samplewraps[k],samplewraps[k],samplewraps[k], 
				seed
			);
		heighttex.d[4*(i+j*heighttex.w)] = (unsigned char)clampf(
				256 * val,0,255);
		int color = ncolors-1;
		for(;color > 0 && transitionpoints[color-1] > heighttex.d[4*(i+j*heighttex.w)]; color--);
		//TODO add self shadowing
		colortex.d[4*(i+j*heighttex.w)  ] = colortable[color*3+0];
		colortex.d[4*(i+j*heighttex.w)+1] = colortable[color*3+1];
		colortex.d[4*(i+j*heighttex.w)+2] = colortable[color*3+2];
	}
	//calculate self-shadowing.
	const int ISVERT = SUNDIR/2;
	const int ISNEG = SUNDIR%2;
	for(int i = 0; i < heighttex.w; i++)
	for(int j = 0; j < heighttex.h; j++){
		unsigned char height = (uchar)heighttex.d[4*(i+j*heighttex.w)];
		int max_searched_pixels = (float)(255 - (int)height) / SUN_CLIMB_COEFF;
		if(height == 255) continue;
		unsigned char testheights[max_searched_pixels];
		for(int b = 0; b < max_searched_pixels; b++)
			testheights[b] = clampf((float)height + (float)(b+1) * SUN_CLIMB_COEFF,0,255);

		//Written specifically for SUNDIR = 0
		if(SUNDIR == 0)
		for(int v = 0; v < max_searched_pixels; v++){
			if((uchar)heighttex.d[4*( ((i+v+1)%heighttex.w)+j*heighttex.w)] > testheights[v])
				{
					colortex.d[4*(i+j*heighttex.w)+0] /= 2;
					colortex.d[4*(i+j*heighttex.w)+1] /= 2;
					colortex.d[4*(i+j*heighttex.w)+2] /= 2;
				}
		}
	}
	printf("\nHEIGHT WIDTH = %d", heighttex.w);
	printf("\nCOLOR WIDTH = %d", colortex.w);
	printf("\nLOADED THE SHIT!!!\n");fflush(stdout);
	init();//ainit();
	cWin("Sprite Rendering!",300,300,WIDTH * RATIO,HEIGHT * RATIO,0);
	cRend();cSurf(WIDTH,HEIGHT);srand(time(NULL));
	lSPal(&mypal);
	printf("\nsurf->w = %d",surf->w);
	fflush(stdout);
	
	for(;!shouldQuit;){
		camera.ang+=0.01;
		
		if(camera.ang > 2 * 3.14159)
			camera.ang -= 2*3.14159;
		camera.p.d[0]+= 2;
		if(camera.p.d[0] > hmapw)
			camera.p.d[0]-=hmapw;
		
		t += 16.66666/1000;
		camera.p.d[2] = 230 + 100 * sinf(1.4 * t);
		camera.horizon = HEIGHT/2 + 50 * sinf(1.3 * t);
		blitback(&mybackspr,sppos[0],sppos[1]);
		
		m = 20;
		hshift(wavy);
		m = 40;
		vshift(wavy);
		//TODO Render terrain.
		Render();
		
		rensp(&mysprite,sppos[0],sppos[1]);
		ev(EVENT_HANDLER);
		upd();
		if(t>1000)t-=1000;
	}
	freesp(&mybackspr);
	freesp(&mysprite);
	freesp(&heighttex);
	freesp(&colortex);
	clean();
	return 0;
}
