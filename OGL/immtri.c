
#define USE_API_GL
#define CHAD_API_IMPL
#define USE_MIXER
#include "include/gl_helpers.h"
//Depends: gl_helpers.c
#include <time.h>
#include <string.h>
/*
USING API.H TO INTERACT WITH OPENGL
If opengl is active, the api.h rendering calls will do nothing.
*/

int WIDTH = 640;
int HEIGHT = 480;





int shouldQuit = 0;
samp* mysamp = NULL;
//void events(SDL_Event* e){
//	switch(e->type){
BEGIN_EVENT_HANDLER
		case SDL_KEYDOWN:
			if(E_KEYSYM == SDLK_f) printf("\nRespects!");
			if(E_KEYSYM == SDLK_q) shouldQuit = 1;
			if(E_KEYSYM == SDLK_g) aplay(mysamp,0);
		break;
		
		case SDL_WINDOWEVENT:
			if(E_WINEVENT == SDL_WINDOWEVENT_RESIZED ||
			E_WINEVENT == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				WIDTH = E_WINW;
				HEIGHT = E_WINH;
				cSurf(WIDTH,HEIGHT); 
			}
		break;
		
END_EVENT_HANDLER
//	}
//}
int main(int argc, char** argv)
{
	initFont("GKBAF16x16.bmp");
	init();ainit();
	char* lastarg = argv[0];
	for(int i = 1; i < argc; i++){
		if(!strcmp(lastarg,"-w"))
			WIDTH = strtoull(argv[i],0,10);
		if(!strcmp(lastarg,"-h"))
			HEIGHT = strtoull(argv[i],0,10);
		lastarg = argv[i];
	}
	cWin("GL on CCHADS API!",300,300,WIDTH,HEIGHT,0);
	//rend = NULL;
	cRend();
	mysamp = lwav("Zube.wav");
	//surf = NULL;
	cSurf(WIDTH,HEIGHT); 
	srand(time(NULL));
	initGL(WIDTH,HEIGHT); //MUST BE INVOKED **AFTER** CSURF!!!
	basicGLConfig();
	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	int depth = surf->pitch/surf->w;
	printf("\ndepth = %d\n",depth);
	double t = 0;
	glViewport(0,0,WIDTH,HEIGHT);
	for (;shouldQuit < 1;) {
			t+=0.016666f;
			ev(EVENT_HANDLER);
			glEnable(GL_DEPTH_TEST);
			clear(0); //clear to completely transparent black.
			glMatrixMode(GL_MODELVIEW);
			clearGL();
			glLoadIdentity();
			glPushMatrix();
			glRotatef(t*30,0,0,1);
			glBegin(GL_TRIANGLES);
				glColor3f(0.2, 0.2, 1.0);
				glVertex3f(-0.8,-0.8,0.2); 
				glVertex3f(0.8,-0.8,0.2);
				glVertex3f(0,1.2,0.2);
				glColor3f(1.0, 0.2, 0.2);
			glEnd();
			glPopMatrix();
			swapGL();
			for(int x = 0; x < surf->w; x++)
			for(int y = 0; y < surf->h; y++)
			{
				unsigned char* datum = (unsigned char*)surf->pixels;
				datum += depth * (x + y * surf->w);
				datum[G_] += rand()%100 > 50? rand()%3 : -1 * rand()%3;
				//datum[1] += rand()%100 > 50? rand()%3 : -1 * rand()%3;
				//datum[2] += rand()%100 > 50? rand()%3 : -1 * rand()%3;
				//datum[R_] = 0;
				//datum[B_] = 0;
				datum[A_] = datum[G_]>128?255:0;
			}
			drawText(0,0,
					"C CHADS RULE!!!!",
					255,255,255
					);
			//glDisable(GL_DEPTH_TEST);
			endFrame(); //REQUIRES THAT THE MODELVIEW BE PROPERLY POPPED
	}
	Mix_FreeChunk(mysamp);
	acleanup();
	freeFont();
	clean();

	return EXIT_SUCCESS;
}
