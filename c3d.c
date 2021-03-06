#define CHAD_API_IMPL
#define USE_MIXER
#define CHAD_MATH_IMPL
#include "include/3dMath.h"
#include "include/api.h"
#include <time.h>
unsigned int WIDTH = 640;
unsigned int HEIGHT = 480;
#define RATIO 2
#define NBUB 300 // bubbles.
#define BUBLIFE 300 // frames.
/*3D TRIANGLE RENDERING DEMO
*/
float c3d[202 * 9] = {
/* Triangle 0 = */
-1.093097, 1.000000, -0.000000,
-1.093097, -1.000000, 1.000000,
-1.093097, -1.000000, -0.000000,
/* Triangle 3 = */
-1.093097, 1.000000, 1.000000,
-1.350445, 1.000000, 2.960648,
-0.134523, 1.000000, 2.026357,
/* Triangle 6 = */
-1.093097, -1.000000, -0.000000,
-2.626513, -1.000000, 1.000000,
-2.626513, -1.000000, -0.000000,
/* Triangle 9 = */
-2.626513, 1.000000, -0.000000,
-1.093097, 1.000000, 1.000000,
-1.093097, 1.000000, -0.000000,
/* Triangle 12 = */
-2.626513, 1.000000, 1.000000,
-2.626513, -1.000000, -0.000000,
-2.626513, -1.000000, 1.000000,
/* Triangle 15 = */
-0.134523, 1.000000, 2.026357,
1.536813, -1.000000, 2.496560,
-0.134523, -1.000000, 2.026357,
/* Triangle 18 = */
-1.093097, 1.000000, 1.000000,
-0.134523, -1.000000, 2.026357,
-1.093097, -1.000000, 1.000000,
/* Triangle 21 = */
-2.626513, -1.000000, 1.000000,
-1.350445, 1.000000, 2.960648,
-2.626513, 1.000000, 1.000000,
/* Triangle 24 = */
-1.093097, -1.000000, 1.000000,
-1.350445, -1.000000, 2.960648,
-2.626513, -1.000000, 1.000000,
/* Triangle 27 = */
1.536813, -1.000000, 2.496560,
2.723612, -1.000000, 2.884702,
0.917331, -1.000000, 3.899275,
/* Triangle 30 = */
-0.134523, -1.000000, 2.026357,
0.917331, -1.000000, 3.899275,
-1.350445, -1.000000, 2.960648,
/* Triangle 33 = */
-1.350445, -1.000000, 2.960648,
0.917331, 1.000000, 3.899275,
-1.350445, 1.000000, 2.960648,
/* Triangle 36 = */
-0.134523, 1.000000, 2.026357,
0.917331, 1.000000, 3.899275,
1.536813, 1.000000, 2.496560,
/* Triangle 39 = */
2.450636, 1.000000, 1.375778,
2.723612, -1.000000, 2.884702,
2.450636, -1.000000, 1.375778,
/* Triangle 42 = */
0.917331, -1.000000, 3.899275,
2.723612, 1.000000, 2.884702,
0.917331, 1.000000, 3.899275,
/* Triangle 45 = */
1.536813, 1.000000, 2.496560,
2.723612, 1.000000, 2.884702,
2.450636, 1.000000, 1.375778,
/* Triangle 48 = */
1.536813, 1.000000, 2.496560,
2.450636, -1.000000, 1.375778,
1.536813, -1.000000, 2.496560,
/* Triangle 51 = */
-1.093097, -1.000000, -1.000000,
-1.093097, 1.000000, -0.000000,
-1.093097, -1.000000, -0.000000,
/* Triangle 54 = */
-1.093097, 1.000000, -1.000000,
-1.350445, 1.000000, -2.960649,
-2.626513, 1.000000, -1.000000,
/* Triangle 57 = */
-2.626513, -1.000000, -1.000000,
-1.093097, -1.000000, -0.000000,
-2.626513, -1.000000, -0.000000,
/* Triangle 60 = */
-1.093097, 1.000000, -1.000000,
-2.626513, 1.000000, -0.000000,
-1.093097, 1.000000, -0.000000,
/* Triangle 63 = */
-2.626513, -1.000000, -0.000000,
-2.626513, 1.000000, -1.000000,
-2.626513, -1.000000, -1.000000,
/* Triangle 66 = */
1.536813, -1.000000, -2.496561,
-0.134523, 1.000000, -2.026358,
-0.134523, -1.000000, -2.026358,
/* Triangle 69 = */
-0.134523, -1.000000, -2.026358,
-1.093097, 1.000000, -1.000000,
-1.093097, -1.000000, -1.000000,
/* Triangle 72 = */
-1.350445, 1.000000, -2.960649,
-2.626513, -1.000000, -1.000000,
-2.626513, 1.000000, -1.000000,
/* Triangle 75 = */
-1.350445, -1.000000, -2.960649,
-1.093097, -1.000000, -1.000000,
-2.626513, -1.000000, -1.000000,
/* Triangle 78 = */
2.723612, -1.000000, -2.884702,
1.536813, -1.000000, -2.496561,
0.917331, -1.000000, -3.899275,
/* Triangle 81 = */
0.917331, -1.000000, -3.899275,
-0.134523, -1.000000, -2.026358,
-1.350445, -1.000000, -2.960649,
/* Triangle 84 = */
0.917331, 1.000000, -3.899275,
-1.350445, -1.000000, -2.960649,
-1.350445, 1.000000, -2.960649,
/* Triangle 87 = */
-0.134523, 1.000000, -2.026358,
0.917331, 1.000000, -3.899275,
-1.350445, 1.000000, -2.960649,
/* Triangle 90 = */
2.723612, -1.000000, -2.884702,
2.450636, 1.000000, -1.375779,
2.450636, -1.000000, -1.375779,
/* Triangle 93 = */
2.723612, 1.000000, -2.884702,
0.917331, -1.000000, -3.899275,
0.917331, 1.000000, -3.899275,
/* Triangle 96 = */
1.536813, 1.000000, -2.496561,
2.723612, 1.000000, -2.884702,
0.917331, 1.000000, -3.899275,
/* Triangle 99 = */
2.450636, -1.000000, -1.375779,
1.536813, 1.000000, -2.496561,
1.536813, -1.000000, -2.496561,
/* Triangle 102 = */
-1.093097, 1.000000, -0.000000,
-1.093097, 1.000000, 1.000000,
-1.093097, -1.000000, 1.000000,
/* Triangle 105 = */
-1.093097, 1.000000, 1.000000,
-2.626513, 1.000000, 1.000000,
-1.350445, 1.000000, 2.960648,
/* Triangle 108 = */
-1.093097, -1.000000, -0.000000,
-1.093097, -1.000000, 1.000000,
-2.626513, -1.000000, 1.000000,
/* Triangle 111 = */
-2.626513, 1.000000, -0.000000,
-2.626513, 1.000000, 1.000000,
-1.093097, 1.000000, 1.000000,
/* Triangle 114 = */
-2.626513, 1.000000, 1.000000,
-2.626513, 1.000000, -0.000000,
-2.626513, -1.000000, -0.000000,
/* Triangle 117 = */
-0.134523, 1.000000, 2.026357,
1.536813, 1.000000, 2.496560,
1.536813, -1.000000, 2.496560,
/* Triangle 120 = */
-1.093097, 1.000000, 1.000000,
-0.134523, 1.000000, 2.026357,
-0.134523, -1.000000, 2.026357,
/* Triangle 123 = */
-2.626513, -1.000000, 1.000000,
-1.350445, -1.000000, 2.960648,
-1.350445, 1.000000, 2.960648,
/* Triangle 126 = */
-1.093097, -1.000000, 1.000000,
-0.134523, -1.000000, 2.026357,
-1.350445, -1.000000, 2.960648,
/* Triangle 129 = */
1.536813, -1.000000, 2.496560,
2.450636, -1.000000, 1.375778,
2.723612, -1.000000, 2.884702,
/* Triangle 132 = */
-0.134523, -1.000000, 2.026357,
1.536813, -1.000000, 2.496560,
0.917331, -1.000000, 3.899275,
/* Triangle 135 = */
-1.350445, -1.000000, 2.960648,
0.917331, -1.000000, 3.899275,
0.917331, 1.000000, 3.899275,
/* Triangle 138 = */
-0.134523, 1.000000, 2.026357,
-1.350445, 1.000000, 2.960648,
0.917331, 1.000000, 3.899275,
/* Triangle 141 = */
2.450636, 1.000000, 1.375778,
2.723612, 1.000000, 2.884702,
2.723612, -1.000000, 2.884702,
/* Triangle 144 = */
0.917331, -1.000000, 3.899275,
2.723612, -1.000000, 2.884702,
2.723612, 1.000000, 2.884702,
/* Triangle 147 = */
1.536813, 1.000000, 2.496560,
0.917331, 1.000000, 3.899275,
2.723612, 1.000000, 2.884702,
/* Triangle 150 = */
1.536813, 1.000000, 2.496560,
2.450636, 1.000000, 1.375778,
2.450636, -1.000000, 1.375778,
/* Triangle 153 = */
-1.093097, -1.000000, -1.000000,
-1.093097, 1.000000, -1.000000,
-1.093097, 1.000000, -0.000000,
/* Triangle 156 = */
-1.093097, 1.000000, -1.000000,
-0.134523, 1.000000, -2.026358,
-1.350445, 1.000000, -2.960649,
/* Triangle 159 = */
-2.626513, -1.000000, -1.000000,
-1.093097, -1.000000, -1.000000,
-1.093097, -1.000000, -0.000000,
/* Triangle 162 = */
-1.093097, 1.000000, -1.000000,
-2.626513, 1.000000, -1.000000,
-2.626513, 1.000000, -0.000000,
/* Triangle 165 = */
-2.626513, -1.000000, -0.000000,
-2.626513, 1.000000, -0.000000,
-2.626513, 1.000000, -1.000000,
/* Triangle 168 = */
1.536813, -1.000000, -2.496561,
1.536813, 1.000000, -2.496561,
-0.134523, 1.000000, -2.026358,
/* Triangle 171 = */
-0.134523, -1.000000, -2.026358,
-0.134523, 1.000000, -2.026358,
-1.093097, 1.000000, -1.000000,
/* Triangle 174 = */
-1.350445, 1.000000, -2.960649,
-1.350445, -1.000000, -2.960649,
-2.626513, -1.000000, -1.000000,
/* Triangle 177 = */
-1.350445, -1.000000, -2.960649,
-0.134523, -1.000000, -2.026358,
-1.093097, -1.000000, -1.000000,
/* Triangle 180 = */
2.723612, -1.000000, -2.884702,
2.450636, -1.000000, -1.375779,
1.536813, -1.000000, -2.496561,
/* Triangle 183 = */
0.917331, -1.000000, -3.899275,
1.536813, -1.000000, -2.496561,
-0.134523, -1.000000, -2.026358,
/* Triangle 186 = */
0.917331, 1.000000, -3.899275,
0.917331, -1.000000, -3.899275,
-1.350445, -1.000000, -2.960649,
/* Triangle 189 = */
-0.134523, 1.000000, -2.026358,
1.536813, 1.000000, -2.496561,
0.917331, 1.000000, -3.899275,
/* Triangle 192 = */
2.723612, -1.000000, -2.884702,
2.723612, 1.000000, -2.884702,
2.450636, 1.000000, -1.375779,
/* Triangle 195 = */
2.723612, 1.000000, -2.884702,
2.723612, -1.000000, -2.884702,
0.917331, -1.000000, -3.899275,
/* Triangle 198 = */
1.536813, 1.000000, -2.496561,
2.450636, 1.000000, -1.375779,
2.723612, 1.000000, -2.884702,
/* Triangle 201 = */
2.450636, -1.000000, -1.375779,
2.450636, 1.000000, -1.375779,
1.536813, 1.000000, -2.496561};
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
	case SDL_WINDOWEVENT:
		if(E_WINEVENT == SDL_WINDOWEVENT_RESIZED ||
		E_WINEVENT == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			WIDTH = E_WINW/RATIO;
			HEIGHT = E_WINH/RATIO;
			cSurf(WIDTH,HEIGHT);
		}
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
const float znear = 10.0;
const float zfar = 1000.0;
//const float zfar2 = 2000.0;
#define zm ((zfar - znear)/2.0+znear)
int wavy(int i){
	return m * sinf(t + (float)i/10);
}

void rotatemodel(pmod* model, 
	float a, float b, float c){
	mat4 rm = rotate(
		(vec3){
			.d[0]=a,
			.d[1]=b,
			.d[2]=c
		}
	);
	for(size_t i = 0; i < model->npoints;i++){
		float* x = model->d+i*3;
		float* y = model->d+i*3+1;
		float* z = model->d+i*3+2;
		vec4 v = (vec4){
			.d[0]= *x,
			.d[1]= *y,
			.d[2]= *z,
			.d[3]= 1,
		};
		v = mat4xvec4(rm, v);
		*x = v.d[0]; *y = v.d[1]; *z = v.d[2];
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
			*x	*= sx;
			*y	*= sy;
			*z	*= sz;
		//*x += surf->w/2.0; *y += surf->h/2.0; *z +=zm;
	}
}
void applyperspective(pmod* model){
	mat4 pm = perspective(90, (float)WIDTH/(float)HEIGHT, 1.0, 500.0);
	for(size_t i = 0; i < model->npoints; i++){
		float x = model->d[i*3];
		float y = model->d[i*3+1];
		float z = model->d[i*3+2];
		vec4 a = (vec4){
			.d[0] = x,
			.d[1] = y,
			.d[2] = z,
			.d[3] = 1.0
		};
		a = mat4xvec4(pm,a);
		a.d[0] /= a.d[3];
		a.d[1] /= a.d[3];
		a.d[2] /= a.d[3];
		//model->d[i*3+2] = a.d[2];
		model->d[i*3] = a.d[0];
		model->d[i*3+1] = a.d[1];
	}
}
void applyViewport(pmod* model){
	for(size_t i = 0; i < model->npoints; i++){
		float z = model->d[i*3+2];
		float x = model->d[i*3];
		float y = model->d[i*3+1];
		vec3 a = (vec3){
			.d[0] = x,
			.d[1] = y,
			.d[2] = z
		};
		a=viewport(WIDTH,HEIGHT,a);
		model->d[i*3] = a.d[0];
		model->d[i*3+1] = a.d[1];
		model->d[i*3+2] = a.d[2];
		//printf("\nz[%zu] = %f",i,a.d[2]);
	}
}
int main()
{
	palette mypal;lSPal(&mypal);
	sprite mysprite;
	sprite mybackspr;
    pmod cube_original, cube;
    cube_original.npoints = 202*3;
    cube.npoints = 202 * 3;
    cube_original.d = c3d;
    cube.d = malloc(202*9*sizeof(float));
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
	free(backspr);
	initFont("GKBAF16x16.bmp");
	init();ainit();
	cWin("C Chads, 3D Rendering!",100,100,WIDTH * RATIO,HEIGHT * RATIO,1);
	cRend();
	puts("\nBefore cSurf");
	cSurf(WIDTH,HEIGHT);
	puts("\nAfter cSurf");
	srand(time(NULL));
	lSPal(&mypal);
	mytrack = lmus("Music by C Chads/WWGW.wav");
	if(!mytrack){
		puts("\nError loading music.\n");
	}
	mplay(mytrack, -1, 1000);
	fflush(stdout);
	//Make a cube.
	for(;!shouldQuit;){
		t+=0.016666;
		memcpy(cube.d,cube_original.d,202*9*sizeof(float));
		scalemodel(&cube,100.0,100.0,100.0);
		rotatemodel(&cube,
			0,
			0,
			3.14159/2);
		rotatemodel(&cube,
		0,
		t*0.7,
		0);
		translatemodel(&cube,
		800.0 * sinf(1.7*t),
		600.0 * cosf(1.4*t),
		zfar);
		applyperspective(&cube);
		applyViewport(&cube);
		resetdbuff(zfar*2);
		blitback(&mybackspr,sppos[0],sppos[1]);
		m = 5;
		hshift(wavy);
		drawText(0,0,
		"C CHADS RULE!!!!",
		128,128,255
		);
		m = 5;
		vshift(wavy);
		rensp(&mysprite,sppos[0],sppos[1]);
		//linemodel(&cube, 255,255,255);
		for(int i = 0; i < 202; i++){
			vec3 a = (vec3){
				.d[0]=cube.d[i*9], 
				.d[1]=cube.d[i*9+1], 
				.d[2]=cube.d[i*9+2]};
			vec3 b = (vec3){
				.d[0]=cube.d[i*9+3], .d[1]=cube.d[i*9+4], .d[2]=cube.d[i*9+5]};
			vec3 c = (vec3){.d[0]=cube.d[i*9+6], .d[1]=cube.d[i*9+7], .d[2]=cube.d[i*9+8]};
			vec3 bma = subv3(b,a);
			vec3 cma = subv3(c,a);
			vec3 crossed = crossv3(bma,cma);
			if(crossed.d[2]<0)
			tri2d(	cube.d[i*9], cube.d[i*9+1], cube.d[i*9+2],
					cube.d[i*9+3], cube.d[i*9+4], cube.d[i*9+5],
					cube.d[i*9+6], cube.d[i*9+7], cube.d[i*9+8],
				i%2?255:0,
				i%2?0:128,
				i%2?0:200);
		}
		ev(EVENT_HANDLER);
		upd();
	}
	free(cube.d);
	//free(cube_original.d);
	freesp(&mybackspr);
	freesp(&mysprite);
	freeFont();
	clean();
	return 0;
}