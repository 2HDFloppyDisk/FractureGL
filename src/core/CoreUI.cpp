#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "core/CoreUI.h"
#include "ui/GUIStyle.h"
#include "utils/LogUtils.h"
#include "gameplay/GameStateHandler.h"
#include "ui/DebugUI.h"
#include "audio/AudioPlayer.h"

// External window object
extern GLFWwindow* window;

// Static instance of the audio player
static AudioPlayer audioPlayer;

// Constructor
CoreUI::CoreUI() {
    // Constructor code if necessary
}

// Destructor
CoreUI::~CoreUI() {
    // Destructor code if necessary
}

// Initialize ImGui
void CoreUI::InitImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Disable saving and loading window state (positions, sizes) to imgui.ini
    io.IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    // Apply the custom SWG style for the UI
    ApplySWGStyle();

    debugMessages.push_back("UI initialized");
}

// Function to calculate the total required window height based on visible ImGui windows
void CoreUI::UpdateMainWindowSize(GLFWwindow* window) {
    float totalHeight = 227.0f;

    // Add height of the Audio Player UI
    if (audioPlayer.IsPlayerUIVisible()) {
        totalHeight += ImGui::GetWindowHeight();  // Adjust this to match the height of the Audio Player UI
    }

    // Add height of the Audio Selection UI if visible
    if (showAudioSelectionUI) {
        totalHeight += ImGui::GetWindowHeight();  // Adjust this to match the height of the Audio Selection UI
    }

    // Add height of the Debug Console UI if visible
    if (showDebugConsoleUI) {
        totalHeight += ImGui::GetWindowHeight();  // Adjust this to match the height of the Debug Console UI
    }

    // Set the main window size (maintain a fixed width, adjust height)
    int currentWidth, currentHeight;
    glfwGetWindowSize(window, &currentWidth, &currentHeight);
    glfwSetWindowSize(window, currentWidth, static_cast<int>(totalHeight));
}

// Function to draw the menu bar
void CoreUI::DrawMenuBar() {
    // Adjust the menu bar height by modifying the style
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 7.0f));  // Adjust the padding (x, y)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0.0f));  // Adjust the minimum height for the menu bar

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Add Audio to Library", nullptr, &showAddAudioLibraryUI);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Skins", nullptr, &showSkinsUI);
            ImGui::MenuItem("Preferences", nullptr, &showPreferencesUI);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Audio Library", nullptr, &showAudioSelectionUI);
            //ImGui::MenuItem("Debug Console UI", nullptr, &showDebugConsoleUI);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("Send Feedback", nullptr, &showFeedbackUI);
            ImGui::MenuItem("About", nullptr, &showAboutmenuUI);
            ImGui::EndMenu();
        }

        // Create the close button
        ImGui::SameLine(ImGui::GetWindowWidth() - 35);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));

        if (ImGui::Button("X", ImVec2(20.0f, 20.0f))) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        ImGui::PopStyleVar(3);
        ImGui::EndMainMenuBar();
    }
    UpdateMainWindowSize(window);
}

// Render ImGui frame
void CoreUI::RenderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DrawMenuBar();

    float currentYPosition = 40.0f;
    float menuBarHeight = ImGui::GetFrameHeight();
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    audioPlayer.ShowPlayerUI();

    if (audioPlayer.IsPlayerUIVisible()) {
        ImGui::SetNextWindowPos(ImVec2(0, currentYPosition));
        currentYPosition += ImGui::GetWindowHeight();
    }

    if (showAudioSelectionUI) {
        ImGui::SetNextWindowPos(ImVec2(0, currentYPosition));
        audioPlayer.RenderAudioSelectionUI();
        currentYPosition += ImGui::GetWindowHeight();
    }

    if (showDebugConsoleUI) {
        ImVec2 audioPlayerSize = ImGui::GetWindowSize();
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(audioPlayerSize));
        RenderDebugUI();
    }

    if (showAddAudioLibraryUI) {
        std::string filePath = audioPlayer.OpenFileDialog();
        if (!filePath.empty()) {
            audioPlayer.AddExternalAudio(filePath);
        }
        showAddAudioLibraryUI = false;
    }

    if (showAboutmenuUI) {
        ShowAboutUI();
    }

    UpdateMainWindowSize(window);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Clean up ImGui resources
void CoreUI::CleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// Show About UI
void CoreUI::ShowAboutUI() {
    ImGui::Begin("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("OGLAmp");
    ImGui::Text("Version: 1.0.0");
    ImGui::Text("Author: Stephen McCarty");
    if (ImGui::Button("Close")) {
        showAboutmenuUI = false;
    }
    ImGui::End();
}
