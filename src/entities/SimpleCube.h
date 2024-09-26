#ifndef SIMPLECUBE_H
#define SIMPLECUBE_H

#include <glm/glm.hpp>

extern unsigned int VAO;  // Use extern
extern unsigned int VBO;  // Use extern
extern unsigned int EBO;  // Use extern

// Declaration of the RenderCube function
void RenderCube(GLuint fogShaderProgram, float cameraX, float cameraY, float cameraZ);
//void RenderCube(glm::mat4 view, glm::mat4 projection);
void InitCube();

#endif
