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

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // Initialize GLFW and GLEW
    window = InitGLFW("OGLAmp", 1280, 720);
    if (!window) return -1;

    if (!InitGLEW()) return -1;

    ShowSplashScreen();

    // Setup ImGui context
    InitImGui(window);

    // Get window dimensions
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Setup scene (shaders, objects, particles)
    SetupScene(cameraManager.GetCamera(), windowWidth, windowHeight);

    FMODManager::getInstance().init();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Update FMOD system
        //FMODManager::getInstance().update();

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
