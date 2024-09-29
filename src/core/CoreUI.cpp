#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "ui/GUIStyle.h"
#include "utils/LogUtils.h"
#include "gameplay/GameStateHandler.h"
#include "ui/DebugUI.h"
#include "audio/AudioPlayer.h"

// External window object
extern GLFWwindow* window;

// Static instance of the audio player
static AudioPlayer audioPlayer;

// Toggle flags for UI components
bool showAudioSelectionUI = false;
bool showDebugConsoleUI = false;
bool showAddAudioLibraryUI = false;
bool showDelAudioLibraryUI = false;
bool showSkinsUI = false;
bool showPreferencesUI = false;

// Initialize ImGui
void InitImGui(GLFWwindow* window) {
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
void UpdateMainWindowSize(GLFWwindow* window) {
    float totalHeight = 227.0f;

    // Base height (e.g., menu bar)
    //totalHeight += 40.0f;  // Adjust this value based on the menu bar's height

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
void DrawMenuBar() {
    // Adjust the menu bar height by modifying the style
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 7.0f));  // Adjust the padding (x, y)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0.0f));  // Adjust the minimum height for the menu bar

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Add Audio to Library", nullptr, &showAddAudioLibraryUI);
            //ImGui::MenuItem("Debug Console UI", nullptr, &showDebugConsoleUI);
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
            ImGui::MenuItem("Send Feedback", nullptr, &showAudioSelectionUI);
            ImGui::MenuItem("About", nullptr, &showDebugConsoleUI);
            ImGui::EndMenu();
        }
        // Position the "X" button to the right side of the menu bar with padding
        ImGui::SameLine(ImGui::GetWindowWidth() - 35);  // Adjust this value for horizontal positioning

        // Add padding above the button
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);  // Adjust this value for vertical padding

        // Add padding around the button using style variables
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));  // Adjust the padding around the button itself

        // Create the close button
        if (ImGui::Button("X", ImVec2(20.0f, 20.0f))) {  // Adjust size as needed
            // Set the window to close
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        // Restore the previous style
        ImGui::PopStyleVar(3);  // Restore both frame padding and window min size

        ImGui::EndMainMenuBar();
    }
    UpdateMainWindowSize(window);
}

// Render ImGui frame
void RenderImGui() {
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw the menu bar at the top of the window
    DrawMenuBar();

    float currentYPosition = 40.0f;  // Start after the menu bar; adjust based on menu bar height

    // Get the height of the menu bar to correctly position other elements
    float menuBarHeight = ImGui::GetFrameHeight();

    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    audioPlayer.ShowPlayerUI();
 
    // Render the Audio Player UI anchored below the menu bar
    if (audioPlayer.IsPlayerUIVisible()) {
        ImGui::SetNextWindowPos(ImVec2(0, currentYPosition));  // Set position for the audio player UI
        //audioPlayer.ShowPlayerUI();
        currentYPosition += ImGui::GetWindowHeight();  // Move down by the height of the audio player UI
    }

    // Render the Audio Selection UI if toggled to visible
    if (showAudioSelectionUI) {
        ImGui::SetNextWindowPos(ImVec2(0, currentYPosition));  // Set position for the audio selection UI
        audioPlayer.RenderAudioSelectionUI();
        currentYPosition += ImGui::GetWindowHeight();  // Move down by the height of the audio selection UI
    }

    // Show the debug console UI to the right of the audio player
    if (showDebugConsoleUI) {
        // Get the position and size of the audio player to position the debug console
        ImVec2 audioPlayerPos = ImGui::GetWindowPos();
        ImVec2 audioPlayerSize = ImGui::GetWindowSize();

        // Set the debug console's position to be on the right of the audio player
        ImGui::SetNextWindowPos(ImVec2(audioPlayerPos.x + audioPlayerSize.x, audioPlayerPos.y));
        ImGui::SetNextWindowSize(ImVec2(300.0f, audioPlayerSize.y));  // Set a fixed size for the debug console

        RenderDebugUI();
    }

    if (showAddAudioLibraryUI) {
        //
    }

    if (showDelAudioLibraryUI) {
        //
    }

    if (showSkinsUI) {
        //
    }

    if (showPreferencesUI) {
        //
    }


    UpdateMainWindowSize(window);

    // Render the ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Clean up ImGui resources
void CleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
