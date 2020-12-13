#include "include/shader.h"
#include <stdio.h>
#include <string.h>
void CheckShaderError(GLuint shader, GLuint flag, int isProgram, const char* errortext)
{
	GLint success = 0;
	GLchar error[2048];error[2047]='\0';
	if(isProgram)
		glGetProgramiv(shader,flag,&success);
	else
		glGetShaderiv(shader,flag,&success);
	if(success == GL_FALSE){
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		printf("\n%s: %s\n",errortext,error);
	}
}
GLuint CreateShader(const char* text, unsigned int type) {
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		fprintf(stderr,"\nError compiling shader type %u\n",type);
		//std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* p[1];
	GLint lengths[1];

	p[0] = text;
	lengths[0] = strlen(text);

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, 0, "Error Compiling Shader");

	return shader;
}

void buildShader(
	shader* shad,
	const char* vs,
	const char* fs
) {
	shad->m_program = glCreateProgram();
	shad->m_vs = CreateShader(vs, GL_VERTEX_SHADER);
	shad->m_fs = CreateShader(fs, GL_FRAGMENT_SHADER);
	glAttachShader(shad->m_program, shad->m_vs);
	glAttachShader(shad->m_program, shad->m_fs);
	// glBindAttribLocation(m_program, 0, "vPosition");
	// glBindAttribLocation(m_program, 1, "vColor");

	glLinkProgram(shad->m_program);
	CheckShaderError(shad->m_program, GL_LINK_STATUS, 1, "Error linking shader program");

	glValidateProgram(shad->m_program);
	CheckShaderError(shad->m_program, GL_VALIDATE_STATUS, 1, "Invalid shader program");
}
void destroyShader(shader* shad){
	glDetachShader(shad->m_program, shad->m_vs);
	glDeleteShader(shad->m_vs);
	glDetachShader(shad->m_program, shad->m_fs);
	glDeleteShader(shad->m_fs);
	glDeleteProgram(shad->m_program);
}
void bind(shader* shad){
	glUseProgram(shad->m_program);
}
GLuint getUniformLocation(shader* shad, const char* name){
	return glGetUniformLocation(shad->m_program,name);
}