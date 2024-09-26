#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>  // For GLFW input handling

void ProcessInput(GLFWwindow* window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

#endif // INPUT_H
