#ifndef SIMPLEARROW_H
#define SIMPLEARROW_H

#include <glm/glm.hpp>

extern unsigned int arrowVAO;  // Use extern
extern unsigned int arrowVBO;  // Use extern
extern unsigned int arrowEBO;  // Use extern

void InitArrow(glm::vec3 cameraPos, glm::vec3 cameraFront);

// Function to render an arrow representing the camera's facing direction
void RenderArrow(GLuint arrowShaderProgram, glm::vec3 cameraPos, glm::vec3 cameraFront);

#endif
