#include "GL/gl.h"
#include "../../include/3dMath.h"
#include "../../include/api.h"
//Helpers for immediate mode OpenGL rendering.
//Immediate mode has a certain elegance to it
//and with display lists, it's *almost* as fast
//as modern shaded GL...
typedef struct{
	float shininess;
	float specular[3];
} phongprops;
GLuint createModelDisplayList(
	vec3* points, uint npoints,
	vec3* colors,
	vec3* normals,
	vec3* texcoords
);
void drawModel(
	vec3* points, uint npoints,
	vec3* colors,
	vec3* normals,
	vec3* texcoords
);
void enableLighting();
void disableLighting();
void configLight(
	GLenum ln,
	vec4 ka,
	vec4 kd,
	vec4 ks,
	vec4 pos
);
GLuint loadRGBTexture(unsigned char* buf,unsigned int w, unsigned int h);
void basicGLConfig();
void initGL(unsigned int w, unsigned int h);
inline void swapGL(){} //COMPAT for TGL
void clearGL();
#define endFrame() {\
glEnable(GL_BLEND);\
glMatrixMode(GL_PROJECTION);\
glLoadIdentity();\
glColor3f(1,1,1);\
upd();\
glDisable(GL_BLEND);\
}
