//#define USE_API_GL
#define CHAD_API_IMPL
#define USE_MIXER
#define CHAD_MATH_IMPL
#include "include/gl_helpers.h"
//Depends: gl_helpers.c
#include <time.h>
#include <string.h>
/*
USING API.H TO INTERACT WITH OPENGL
You can still render to your normal buffer too! Don't worry!
*/

int WIDTH = 640;
int HEIGHT = 480;




vec3 campos = (vec3){.d[0]=0,.d[1]=0,.d[2]=-3};
vec3 camforw = (vec3){.d[0]=0,.d[1]=0,.d[2]=-1};
vec3 camup = (vec3){.d[0]=0,.d[1]=1,.d[2]=0};
uint wasdstate[4] = {0,0,0,0};
int mousex = 0, mousey = 0;
int shouldQuit = 0;
uchar cursorLocked = 0;
samp* mysamp = NULL;
const float mouseratiox = 1.0/500.0f;
const float mouseratioy = 1.0/500.0f;
void rotateCamera(){
	vec3 a;
	a.d[1] = (float)mousex * mouseratiox;
	a.d[2] = (float)mousey * mouseratioy;
	//if(fabsf(camforw.d[0]) < 0.001) camforw.d[0] = 0.001;
	vec3 right = normalizev3(
					crossv3(
						normalizev3(camforw),
						normalizev3(camup)
					)
				);right.d[1]=0;
	vec3 result = rotatev3(camforw,right,-a.d[2]);
	camup = crossv3(right,camforw);
	camforw = rotatev3(result,camup,-a.d[1]);
	//if(fabsf(camforw.d[0]) < 0.001) camforw.d[0] = 0.001;
}


BEGIN_EVENT_HANDLER
		case SDL_KEYDOWN:
			if(E_KEYSYM == SDLK_f) printf("\nRespects!");
			if(E_KEYSYM == SDLK_q) shouldQuit = 1;
			if(E_KEYSYM == SDLK_g) aplay(mysamp,0);
			if(E_KEYSYM == SDLK_w){
				wasdstate[0] = 1;
			}
			if(E_KEYSYM == SDLK_a){
				wasdstate[1] = 1;
			}
			if(E_KEYSYM == SDLK_s){
				wasdstate[2] = 1;
			}
			if(E_KEYSYM == SDLK_d){
				wasdstate[3] = 1;
			}
			if(E_KEYSYM == SDLK_c){
				if(!cursorLocked)cursorLocked = 1;
				else cursorLocked = 0;
			}
		break;
		case SDL_KEYUP:
			if(E_KEYSYM == SDLK_w){
				wasdstate[0] = 0;
			}
			if(E_KEYSYM == SDLK_a){
				wasdstate[1] = 0;
			}
			if(E_KEYSYM == SDLK_s){
				wasdstate[2] = 0;
			}
			if(E_KEYSYM == SDLK_d){
				wasdstate[3] = 0;
			}
		break;
		case SDL_MOUSEMOTION: 
			mousex = E_MOTION.xrel; 
			mousey = E_MOTION.yrel;
			if(cursorLocked)
				rotateCamera();
		break;
		case SDL_WINDOWEVENT:
			if(E_WINEVENT == SDL_WINDOWEVENT_RESIZED ||
			E_WINEVENT == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				WIDTH = E_WINW;
				HEIGHT = E_WINH;
				cSurf(WIDTH,HEIGHT); 
				glViewport(0,0,WIDTH,HEIGHT);
			}
		break;
		case SDL_MOUSEBUTTONDOWN:
			if(E_BUTTON==SDL_BUTTON_LEFT) {
			
			} else if(E_BUTTON==SDL_BUTTON_RIGHT){
				
			}
		break;
END_EVENT_HANDLER
//	}
//}
int main(int argc, char** argv)
{
	char* lastarg = argv[0];
	for(int i = 1; i < argc; i++){
		if(!strcmp("-w",lastarg))
			WIDTH = atoi(argv[i]);
		if(!strcmp("-h",lastarg))
			HEIGHT = atoi(argv[i]);
		lastarg = argv[i];
	}
	initFont("GKBAF16x16.bmp");
	init();ainit();
	cWin("Camera demo",300,300,WIDTH,HEIGHT,0);
	//rend = NULL;
	cRend();
	mysamp = lwav("Zube.wav");
	//surf = NULL;
	cSurf(WIDTH,HEIGHT); 
	srand(time(NULL));
	initGL(WIDTH,HEIGHT); //MUST BE INVOKED **AFTER** CSURF!!!
	basicGLConfig();
	glShadeModel(GL_SMOOTH);
	glViewport(0,0,WIDTH,HEIGHT);
	//glEnable(GL_TEXTURE_2D);
	GLuint tex = 0;
	{
		int sw = 0, sh = 0, sc = 0; //sc goes unused.
		uchar* source_data = stbi_load("tex.jpg", &sw, &sh, &sc, 3);
		if(source_data)
			{tex = loadRGBTexture(source_data, sw, sh);free(source_data);}
	//tex = 
	}
	
	//glEnable(GL_BLEND);
	
	//glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	int depth = surf->pitch/surf->w;
	
	printf("\ndepth = %d\n",depth);
	for (;shouldQuit < 1;) {
			//uTx(); //Marks end of this rendering section.
			//Do our GL rendering here
			
			clear(0); //clear to completely transparent black.
			/*
			drawText(0,0,
					"C CHADS RULE!!!!",
					255,255,255
					);
			*/
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			//gluPerspective(70,(float)WIDTH/(float)HEIGHT,1,512);
			mat4 m = perspective(70,(float)WIDTH/(float)HEIGHT,1,512);
			glLoadMatrixf(m.d);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glPushMatrix(); //Pushing on the LookAt Matrix.
			/*
			gluLookAt(
			campos.d[0],campos.d[1],campos.d[2],
			campos.d[0] + camforw.d[0],
			campos.d[1] + camforw.d[1],
			campos.d[2] + camforw.d[2],
			camup.d[0],camup.d[1],camup.d[2]
			);
			*/
			m = (lookAt(campos,addv3(campos,camforw),camup));
			glLoadMatrixf(m.d);
			vec3 right = normalizev3(
				crossv3(
					normalizev3(camforw),
					normalizev3(camup)
				)
			);
			if(wasdstate[0])
				campos = addv3(campos,
					scalev3(0.1,
						camforw
					)
				);
			if(wasdstate[2])
				campos = addv3(campos,
					scalev3(-0.1,
						camforw
					)
				);
			if(wasdstate[1])
				campos = addv3(campos,
					scalev3(-0.1,
						right
					)
				);
			if(wasdstate[3])
				campos = addv3(campos,
					scalev3(0.1,
						right
					)
				);
			clearGL();
			glEnable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D,tex);
			//glDisable(GL_TEXTURE_2D);
			//printf("\nNew triangle!\n");
			glBegin(GL_TRIANGLES);
				//glColor3f(0,0,1);
				glColor3f(1,0,0);glTexCoord2f(0,0);glVertex3f(-1,-1,-10); 
				glColor3f(0,1,0);glTexCoord2f(1,0);glVertex3f(1,-1,-10);
				glColor3f(0,0,1);glTexCoord2f(0.5,1);glVertex3f(0,1,-10);
				//glColor3f(0,1,0);
			glEnd();
			glPopMatrix(); //The view transform.
			swapGL(); //DUMMY! Compat with TGL!
			drawText(0,0,
				"C CHADS RULE!!!!",
				255,255,255
				);
			ev(EVENT_HANDLER);
			if(cursorLocked)
				captMouse();
			else
				noCaptMouse();
			
			//Render the surface
			//flipVertical(); //Necessary when rendering to the OGL window.
			/*
			glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			*/
			//upd(); //draws the SDl2 surface using glDrawPixels(); because we USE_API_GL
			endFrame();
			//swapGL();//Update GL
	}
	Mix_FreeChunk(mysamp);
	acleanup();
	freeFont();
	clean();

	return EXIT_SUCCESS;
}
