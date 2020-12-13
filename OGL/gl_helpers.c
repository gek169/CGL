#include "include/gl_helpers.h"
void basicGLConfig(){
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
//	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0,0,0,0);
	glClearDepth(1.0f);
	glClearStencil(0);
	glDepthFunc(GL_LEQUAL);
}
void enableLighting(){
	glEnable(GL_LIGHTING);
}
void disableLighting(){
	glDisable(GL_LIGHTING);
}
void configLight( //THIS FUNCTION ALSO DEPENDS ON
//f_ being FLOAT!!!
	GLenum ln,
	vec4 ka,
	vec4 kd,
	vec4 ks,
	vec4 pos
){
	glLightfv(ln,GL_AMBIENT,ka.d);
	glLightfv(ln,GL_DIFFUSE,kd.d);
	glLightfv(ln,GL_SPECULAR,ks.d);
	glLightfv(ln,GL_POSITION,pos.d);
}


void drawModel(
//HUGE important note! these depend on the math library using 
//f_ as float and not double!
//Remember that!
	vec3* points, uint npoints,
	vec3* colors,
	vec3* normals,
	vec3* texcoords
){
	if(!points)return;
	glBegin(GL_TRIANGLES);
	for(uint i = 0; i < npoints;i++){
		if(colors){
			glColor3f(colors[i].d[0],colors[i].d[1],colors[i].d[2]);
		} else glColor3f(1,1,1);
		if(texcoords)
			glTexCoord2f(texcoords[i].d[0],texcoords[i].d[1]);
		else glTexCoord2f(0,0);
		if(normals)
			glNormal3f(normals[i].d[0],normals[i].d[1],normals[i].d[2]);
		glVertex3f(points[i].d[0],points[i].d[1],points[i].d[2]);
	}
	glEnd();
}


void initGL(unsigned int w, unsigned int h){
	//This is mostly a dummy function, but we
	//configure the blend function regardless.
	//It cannot be configured in TGL.
	
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
}

GLuint createModelDisplayList(
//HUGE important note! these depend on the math library using 
//f_ as float and not double!
//Remember that!
	vec3* points, uint npoints,
	vec3* colors,
	vec3* normals,
	vec3* texcoords
){
	GLenum communism = 0;
	GLuint ret = 0;
	if(!points)return 0;
	ret = glGenLists(1);
	glNewList(ret, GL_COMPILE);
		drawModel(points,npoints,colors,normals,texcoords);
	glEndList();
	communism = glGetError(); //As it so often does.
		if (communism != GL_NO_ERROR)
			{
				puts("\nGL reports an ERROR!");
					if (communism == GL_INVALID_ENUM)
					puts("\n Invalid enum.");
					if (communism == GL_INVALID_OPERATION)
							puts("\n Invalid operation.");
					if (communism == GL_INVALID_FRAMEBUFFER_OPERATION)
							puts("\n Invalid Framebuffer Operation.");
					if (communism == GL_OUT_OF_MEMORY)
					{
						puts("\n Out of memory. You've really done it now.");
						exit(1);
					}
			}
	return ret;
}
void clearGL(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
GLuint loadRGBTexture(unsigned char* buf, unsigned int w, unsigned int h){
	GLuint t = 0;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D,t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGetError();
	glTexImage2D(GL_TEXTURE_2D,0,3,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,buf);
	
	return t;
}