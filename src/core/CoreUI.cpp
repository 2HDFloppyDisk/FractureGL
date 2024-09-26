#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "../ui/GUIStyle.h"
#include "../utils/LogUtils.h"
#include "../gameplay/GameStateHandler.h"
#include "../ui/DebugUI.h"




extern GLFWwindow* window;

// Initialize ImGui
void InitImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    // Apply the custom SWG style for the UI
    ApplySWGStyle();  // Ensure the custom style is applied

    debugMessages.push_back("ImGui initialized");
}

// Render ImGui frame
void RenderImGui() {
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Render Debug UI and other ImGui elements
    RenderDebugUI();

    // Get window dimensions
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Render background and particles
    //RenderGradientBackground(windowWidth, windowHeight);
    //RenderDustParticles(windowWidth, windowHeight);
    //RenderFogParticles(windowWidth, windowHeight);

    // Handle game state and render appropriate UI/game elements
    HandleGameState();

    // Finalize ImGui rendering
    ImGui::Render();

    // Set up the OpenGL viewport and clear the screen
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render ImGui draw data
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


// Cleanup ImGui resources
void CleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    debugMessages.push_back("ImGui cleaned up");
}
