#include "core/includes.h"

GameState currentState = GameState::MAIN_MENU;

// Global variables for tracking camera and mouse position
CameraManager cameraManager;  // No need for a pointer anymore
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Declare the UI rendering function
void RenderDebugUI();

// Global reference to the GLFW window
GLFWwindow* window = nullptr;

bool isDraggingWindow = false;
double dragOffsetX, dragOffsetY;

void HandleWindowMovement(GLFWwindow* window) {
    if (isDraggingWindow) {
        // Get the current mouse position
        double currentMouseX, currentMouseY;
        glfwGetCursorPos(window, &currentMouseX, &currentMouseY);

        // Calculate the new window position using the initial offset
        int newWindowX = static_cast<int>(currentMouseX - dragOffsetX);
        int newWindowY = static_cast<int>(currentMouseY - dragOffsetY);

        // Set the new window position
        glfwSetWindowPos(window, newWindowX, newWindowY);
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // Initialize GLFW and GLEW
    if (!glfwInit()) return -1;

    // Set window hints before creating the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Disable window decorations (borderless, no title bar)
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // Enable or disable window resizing as desired
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);  // Set to GLFW_FALSE if you want to prevent resizing

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(500, 600, "OGLAmp", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (!InitGLEW()) return -1;

    //ShowSplashScreen();

    // Setup ImGui context
    InitImGui(window);

    // Get window dimensions
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Setup scene (shaders, objects, particles)
    SetupScene(cameraManager.GetCamera(), windowWidth, windowHeight);

    FMODManager::getInstance().init();

    // Initial window size calculation
    UpdateMainWindowSize(window);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Get the current mouse state
        int mouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Check if the user clicked on the menu bar region (adjust y-value based on menu bar height)
        if (mouseButtonState == GLFW_PRESS && mouseY >= 0 && mouseY <= 30) { // Assuming the menu bar height is 30 pixels
            if (!isDraggingWindow) {
                // Begin dragging
                isDraggingWindow = true;

                // Get the current window position
                int windowX, windowY;
                glfwGetWindowPos(window, &windowX, &windowY);

                // Calculate the offset between the mouse position and the window's top-left corner
                dragOffsetX = mouseX - windowX;
                dragOffsetY = mouseY - windowY;
            }
        }
        else if (mouseButtonState == GLFW_RELEASE) {
            // Stop dragging
            isDraggingWindow = false;
        }

        // Handle window movement if dragging
        HandleWindowMovement(window);


        // Static variable to track the last known state of the audio
        static bool wasPlaying = false;

        // Check the current state of the sound
        bool isCurrentlyPlaying = IsSoundPlaying();

        // Log only if the state has changed
        if (isCurrentlyPlaying && !wasPlaying) {
            debugMessages.push_back("Sound is playing in the background");
        }
        else if (!isCurrentlyPlaying && wasPlaying) {
            debugMessages.push_back("Sound has stopped");
            CleanupSound();  // Release the sound after it finishes playing
        }

        // Update the last known state
        wasPlaying = isCurrentlyPlaying;

        // Update delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        ProcessInput(window, deltaTime);

        // Render the scene
        RenderScene(cameraManager.GetCamera(), fogShaderProgram, arrowShaderProgram);

        // Poll events and render ImGui
        glfwPollEvents();

        RenderImGui();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    FMODManager::getInstance().cleanup();
    CleanupScene();
    CleanupImGui();
    TerminateGLFW();

    return 0;
}
