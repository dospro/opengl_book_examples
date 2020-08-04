#ifndef OPENGL_SHADERS_ERROR_HANDLER_H
#define OPENGL_SHADERS_ERROR_HANDLER_H

#include <GL/glew.h>

void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();


#endif//OPENGL_SHADERS_ERROR_HANDLER_H
