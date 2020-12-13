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
const float znear = 0.0;
const float zfar = 1000.0;
#define zm ((zfar - znear)/2.0+znear)
int wavy(int i){
	return m * sinf(t + (float)i/10);
}

void rotatemodel(pmod* model, 
	float a, float b, float c){
	//Build the rotation matrices
	//float xrm[3][3], yrm[3][3], zrm[3][3];
	float rm[3][3];
	rm[0][0] = cosf(a)*cosf(b);
	rm[0][1] = sinf(a)*cosf(b);
	rm[0][2] = -sinf(b);
	rm[1][0] = cosf(a)*sinf(b)*sinf(c)-sinf(a)*cosf(c);
	rm[1][1] = sinf(a)*sinf(b)*sinf(c)+cosf(a)*cosf(c);
	rm[1][2] = cosf(b)*sinf(c);
	rm[2][0] = cosf(a)*sinf(b)*cosf(c)+sinf(a)*sinf(c);
	rm[2][1] = sinf(a)*sinf(b)*cosf(c)-cosf(a)*sinf(c);
	rm[2][2] = cosf(b)*cosf(c);
	//for(int i = 0; i < 9; i++){xrm[i/9][i%9] = 0;yrm[i/9][i%9] = 0;zrm[i/9][i%9] = 0;}
	//set the ones
	for(size_t i = 0; i < model->npoints;i++){
		float* x = model->d+i*3;
		float* y = model->d+i*3+1;
		float* z = model->d+i*3+2;
		float nx, ny, nz;
		//center.
		//*x -= surf->w/2.0; *y -= surf->h/2.0; *z -=zm;
		//rotate
		nx = rm[0][0]* *x + rm[0][1] * *y + rm[0][2] * *z;
		ny = rm[1][0]* *x + rm[1][1] * *y + rm[1][2] * *z;
		nz = rm[2][0]* *x + rm[2][1] * *y + rm[2][2] * *z;
		*x = nx; *y = ny; *z = nz;
		//move back
		//*x += surf->w/2.0; *y += surf->h/2.0; *z +=zm;
	}
}
void translatemodel(pmod* model, float _x, float _y, float _z){
	for(size_t i = 0; i < model->npoints; i++){
			float* x = model->d+i*3;
			float* y = model->d+i*3+1;
			float* z = model->d+i*3+2;
			*x += _x;
			*y += _y;
			*z += _z;
	}
}
void scalemodel(pmod* model, float sx, float sy, float sz){
	for(size_t i = 0; i < model->npoints; i++){
		float* x = model->d+i*3;
		float* y = model->d+i*3+1;
		float* z = model->d+i*3+2;
		//*x -= surf->w/2.0; *y -= surf->h/2.0; *z -=zm;
			*z	*= sz;
			*y	*= sy;
			*x	*= sx;
		//*x += surf->w/2.0; *y += surf->h/2.0; *z +=zm;
	}
}
void applyperspective(pmod* model, float r){
	for(size_t i = 0; i < model->npoints; i++){
		float z = model->d[i*3+2];
		float x = model->d[i*3];
		float y = model->d[i*3+1];
		z-=znear;
		z/=(zfar-znear);
		z=pow(z,r);
		float xm = surf->w/2.0, ym = surf->h/2.0;
		x-=xm;y-=ym;
		if(z<0)continue;
		x*=(z);
		y*=(z);
		x+=xm;y+=ym;
		model->d[i*3+2] = z;
		model->d[i*3] = x;
		model->d[i*3+1] = y;
	}
}
int main()
{
	palette mypal;lSPal(&mypal);
	sprite mysprite;
	sprite mybackspr;
    pmod cube_original, cube;
    cube_original.npoints = 24; //12 lines!
    cube.npoints = 24; //12 lines!
    cube_original.d = malloc(24*sizeof(float)*3);
    cube.d = malloc(24*sizeof(float)*3);
	track* mytrack = NULL;
	
    //Load the background!
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
	init();ainit();
	cWin("Line Rendering!",100,100,WIDTH * RATIO,HEIGHT * RATIO,0);
	cRend();cSurf(WIDTH,HEIGHT);srand(time(NULL));
	lSPal(&mypal);
	mytrack = lmus("Music by C Chads/WWGW.wav");
	if(!mytrack){
		puts("\nError loading music.\n");
	}
	mplay(mytrack, -1, 1000);
	fflush(stdout);
	//Make a cube.
	{
		float p_[24*3]; //12 lines. 24 points with three floats in each.
		for(uchar i = 0; i < 8; i++){
			p_[i*3] = (i&1)?1:-1;
			p_[i*3+1] = (i&2)?1:-1;
			p_[i*3+2] = (i&4)?+1:-1;
		}
		size_t i = 0;
		//BOTTOM LINES
		//Along X
		memcpy(cube_original.d+(i++)*3, p_+(4*0+2*0+1*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*0+2*0+1*1)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*0+2*1+1*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*0+2*1+1*1)*3, 3*sizeof(float));
		//Along Y
		memcpy(cube_original.d+(i++)*3, p_+(4*0+1*0+2*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*0+1*0+2*1)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*0+1*1+2*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*0+1*1+2*1)*3, 3*sizeof(float));
		//TOP LINES
		//Along X
		memcpy(cube_original.d+(i++)*3, p_+(4*1+2*0+1*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+2*0+1*1)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+2*1+1*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+2*1+1*1)*3, 3*sizeof(float));
		//Along Y
		memcpy(cube_original.d+(i++)*3, p_+(4*1+1*0+2*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+1*0+2*1)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+1*1+2*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+1*1+2*1)*3, 3*sizeof(float));
		//VERTICAL LINES
		//0,0
		memcpy(cube_original.d+(i++)*3, p_+(4*0+2*0+1*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+2*0+1*0)*3, 3*sizeof(float));
		//0,1
		memcpy(cube_original.d+(i++)*3, p_+(4*0+2*0+1*1)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+2*0+1*1)*3, 3*sizeof(float));
		//1,0
		memcpy(cube_original.d+(i++)*3, p_+(4*0+2*1+1*0)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+2*1+1*0)*3, 3*sizeof(float));
		//1,1
		memcpy(cube_original.d+(i++)*3, p_+(4*0+2*1+1*1)*3, 3*sizeof(float));
		memcpy(cube_original.d+(i++)*3, p_+(4*1+2*1+1*1)*3, 3*sizeof(float));
		memcpy(cube.d,cube_original.d,24*3*sizeof(float));
	}
	for(;!shouldQuit;){
		t+=0.016666;
		memcpy(cube.d,cube_original.d,24*3*sizeof(float));
		scalemodel(&cube,HEIGHT/4.0,HEIGHT/4.0,HEIGHT/4.0);
		rotatemodel(&cube,t,0.5*t,3*t);
		translatemodel(&cube,
		0.5*WIDTH + 1.2*WIDTH*(sinf(1.13*t)),
		0.5*HEIGHT + 1.2*HEIGHT*(cosf(2.89*t)),
		zm+(znear-zfar)/5.0*cosf(t));
		
		applyperspective(&cube,1.0);
		resetdbuff(0);
		blitback(&mybackspr,sppos[0],sppos[1]);
		m = 20;
		hshift(wavy);
		m = 40;
		vshift(wavy);
		rensp(&mysprite,sppos[0],sppos[1]);
		linemodel(&cube, 255,255,255);
		ev(EVENT_HANDLER);
		upd();
	}
	free(cube.d);
	free(cube_original.d);
	freesp(&mybackspr);
	freesp(&mysprite);
	clean();
	return 0;
}
