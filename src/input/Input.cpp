#include <GLFW/glfw3.h>
#include "../camera/Camera.h"
#include "../input/Input.h"
#include "../camera/CameraManager.h"
#include "../gameplay/GameStateHandler.h"

// Global variables for tracking mouse position
float lastX = 1280.0f / 2.0f;
float lastY = 720.0f / 2.0f;
bool firstMouse = true;

// External cameraManager instance (ensure it's declared as extern elsewhere if needed)
extern CameraManager cameraManager;

// Process keyboard inputs for movement (WASD)
void ProcessInput(GLFWwindow* window, float deltaTime) {
    Camera& camera = cameraManager.GetCamera();  // Get the current camera instance from CameraManager

    if (currentState == GameState::PLAYING) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard('W', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard('S', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard('A', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard('D', deltaTime);
    }
}

// Process mouse input for looking around
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera& camera = cameraManager.GetCamera();  // Get the current camera instance from CameraManager

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
