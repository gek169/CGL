#include "GL3/gl3.h"
#include "GL3/gl3w.h"
#include "../../include/3dMath.h"
#define NULL_SHADER (shader){.m_program=0,.m_vs=0,.m_fs=0}
typedef struct{
	GLuint m_program, m_vs, m_fs;
} shader;
void CheckShaderError(GLuint shader, GLuint flag, int isProgram, const char* errortext); //Prints directly to stdout
void buildShader(
	shader* shad,
	const char* vs,
	const char* fs
);
void destroyShader(shader* shad);
GLuint CreateShader(const char* text, unsigned int type);
void bind(shader* shad);
GLuint getUniformLocation(shader* shad, const char* name);