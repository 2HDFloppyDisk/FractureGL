#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../utils/LogUtils.h"

// Function declarations for initializing and terminating GLFW and GLEW
GLFWwindow* InitGLFW(const char* windowTitle, int width, int height);
bool InitGLEW();
void TerminateGLFW();

#endif // GLUTILS_H
