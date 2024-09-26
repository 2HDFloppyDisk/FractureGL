#ifndef CORE_UI_H
#define CORE_UI_H

#include <GLFW/glfw3.h>

void InitImGui(GLFWwindow* window);
void RenderImGui();
void CleanupImGui();

#endif // CORE_UI_H
