#ifndef RENDER_H
#define RENDER_H

#include "camera/CameraManager.h"  // Include CameraManager instead of Camera directly

extern GLuint fogShaderProgram;
extern GLuint arrowShaderProgram;
extern float lastX, lastY;
extern bool firstMouse;
extern unsigned int VAO;  // Use extern
extern unsigned int VBO;  // Use extern
extern unsigned int EBO;  // Use extern

void CheckOpenGLError(const std::string& message);

// Function to initialize rendering
void InitRendering();

// Function to handle rendering the scene
void RenderScene(Camera& camera, GLuint fogShaderProgram, GLuint arrowShaderProgram);

// Function to handle setting up shaders, objects, and particles
void SetupScene(Camera& camera, int windowWidth, int windowHeight);

// Function to clean up the scene resources
void CleanupScene();

#endif // RENDER_H
