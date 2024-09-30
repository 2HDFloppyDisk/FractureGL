#include "core/includes.h"
#include <fstream>  // Added for file handling
#include <sstream>  // Added for string parsing

GameState currentState = GameState::MAIN_MENU;
CoreUI coreUI;

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

// Function to get the screen's dimensions
void GetScreenSize(int& width, int& height) {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    width = mode->width;
    height = mode->height;
}

// Function to center the window
void CenterWindow(GLFWwindow* window) {
    int screenWidth, screenHeight;
    GetScreenSize(screenWidth, screenHeight);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    glfwSetWindowPos(window, posX, posY);
}

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
    // identify the application instance
    HANDLE hMutex = CreateMutex(NULL, TRUE, L"OGLAmp_Mutex");
    if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS) {
        // If the mutex already exists, an instance of the application is running
        if (hMutex) {
            CloseHandle(hMutex);
        }
        return 0;  // Exit the application
    }



    // Initialize GLFW and GLEW
    if (!glfwInit()) return -1;

    // Set window hints before creating the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);  // Set to GLFW_FALSE if you want to prevent resizing

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(500, 600, "OGLAmp", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Load the last window position from config.ini or center the window if it’s the first launch
    int lastPosX, lastPosY;
    if (!LoadWindowPositionFromConfig(lastPosX, lastPosY)) {
        CenterWindow(window);  // Center the window if config.ini is not found or cannot be loaded
    }
    else {
        glfwSetWindowPos(window, lastPosX, lastPosY);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (!InitGLEW()) return -1;

    //ShowSplashScreen();

    // Setup ImGui context
    coreUI.InitImGui(window);

    // Get window dimensions
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Setup scene (shaders, objects, particles)
    SetupScene(cameraManager.GetCamera(), windowWidth, windowHeight);

    FMODManager::getInstance().init();

    // Initial window size calculation
    coreUI.UpdateMainWindowSize(window);

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

        coreUI.RenderImGui();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Save the window's current position to config.ini before closing
    SaveWindowPositionToConfig(window);

    // Cleanup
    FMODManager::getInstance().cleanup();
    CleanupScene();
    coreUI.CleanupImGui();
    TerminateGLFW();
    // Release the mutex when the application exits
    CloseHandle(hMutex);

    return 0;
}
