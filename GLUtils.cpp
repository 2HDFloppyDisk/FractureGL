#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LogUtils.h"

GLFWwindow* InitGLFW(const char* windowTitle, int width, int height) {
    if (!glfwInit()) {
        debugMessages.push_back("Failed to initialize GLFW");
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr);
    if (!window) {
        debugMessages.push_back("Failed to create GLFW window");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    return window;
}

bool InitGLEW() {
    if (glewInit() != GLEW_OK) {
        debugMessages.push_back("Failed to initialize GLEW");
        return false;
    }
    return true;
}

void TerminateGLFW() {
    glfwTerminate();
    debugMessages.push_back("GLFW terminated");
}
