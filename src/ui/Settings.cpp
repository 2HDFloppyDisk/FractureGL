#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <vector> 
#include <string>
#include <set>
#include "../ui/Settings.h"
#include "../gameplay/GameStateHandler.h"


extern GameState currentState;
extern GLFWwindow* window;  // Reference to the GLFW window

// Variables to store fullscreen modes and resolutions
static int selectedMode = 0;  // 0: Windowed, 1: Fullscreen, 2: Borderless
static int selectedResolution = 0;
std::vector<std::string> resolutionList;
std::vector<std::pair<int, int>> resolutions;

void GetSupportedResolutions()
{
    resolutionList.clear();
    resolutions.clear();

    std::set<std::pair<int, int>> uniqueResolutions;

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor)
    {
        int count;  // Declare the count variable to store the number of video modes
        const GLFWvidmode* modes = glfwGetVideoModes(primaryMonitor, &count);
        for (int i = 0; i < count; i++)
        {
            int width = modes[i].width;
            int height = modes[i].height;

            // Add the resolution to the set, ensuring only unique entries
            uniqueResolutions.insert({ width, height });
        }

        // Populate the resolutionList and resolutions vector with unique values
        for (const auto& res : uniqueResolutions)
        {
            resolutions.push_back(res);
            resolutionList.push_back(std::to_string(res.first) + "x" + std::to_string(res.second));
        }
    }
}

// Functions to display each settings submenu
void ShowGameplaySettings();
void ShowVideoSettings();
void ShowGraphicsSettings();
void ShowAudioSettings();
void ShowControllerSettings();

void ShowSettingsMenu()
{
    // Get the current window size
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate dynamic widths with a minimum and maximum width
    float leftPanelWidth = windowWidth * 0.2f;  // 20% of the window for the left panel
    float rightPanelWidth = windowWidth * 0.75f;  // 75% of the window for the right panel

    // Set a min and max width to keep the panels from getting too small or too large
    int minPanelWidth = 200;   // Minimum width for both panels
    int maxPanelWidth = 350;   // Maximum width to avoid too much expansion

    // Clamp the panel widths between min and max values
    if (leftPanelWidth < minPanelWidth) leftPanelWidth = minPanelWidth;
    if (leftPanelWidth > maxPanelWidth) leftPanelWidth = maxPanelWidth;

    if (rightPanelWidth < minPanelWidth) rightPanelWidth = minPanelWidth;
    if (rightPanelWidth > maxPanelWidth) rightPanelWidth = maxPanelWidth;

    // Center the wrapper horizontally if possible
    int wrapperXPos = (windowWidth - (leftPanelWidth + rightPanelWidth)) * 0.5f;

    // Left panel for settings navigation
    ImGui::SetNextWindowPos(ImVec2(wrapperXPos, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, windowHeight), ImGuiCond_Always);

    if (ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::Text("Settings");

        ImVec2 buttonSize = ImVec2(leftPanelWidth * 0.85f, 40);  // Dynamically size buttons

        if (ImGui::Button("GAMEPLAY", buttonSize))
        {
            currentState = GameState::SETTINGS_GAMEPLAY;
        }
        if (ImGui::Button("VIDEO", buttonSize))
        {
            currentState = GameState::SETTINGS_VIDEO;
        }
        if (ImGui::Button("GRAPHICS", buttonSize))
        {
            currentState = GameState::SETTINGS_GRAPHICS;
        }
        if (ImGui::Button("AUDIO", buttonSize))
        {
            currentState = GameState::SETTINGS_AUDIO;
        }
        if (ImGui::Button("CONTROLLER", buttonSize))
        {
            currentState = GameState::SETTINGS_CONTROLLER;
        }

        // Back button at the bottom
        ImGui::Dummy(ImVec2(0.0f, 20.0f));  // Add some spacing before the Back button
        if (ImGui::Button("Back", buttonSize))
        {
            currentState = GameState::MAIN_MENU;  // Return to main menu
        }

        ImGui::End();
    }

    // Right panel for settings submenu
    ImGui::SetNextWindowPos(ImVec2(wrapperXPos + leftPanelWidth, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(rightPanelWidth, windowHeight), ImGuiCond_Always);

    if (ImGui::Begin("Settings Submenu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::BeginChild("Content Wrapper", ImVec2(rightPanelWidth, 0), false);

        // Display settings content based on the current state
        if (currentState == GameState::SETTINGS_GAMEPLAY)
        {
            ShowGameplaySettings();
        }
        else if (currentState == GameState::SETTINGS_VIDEO)
        {
            ShowVideoSettings();
        }
        else if (currentState == GameState::SETTINGS_GRAPHICS)
        {
            ShowGraphicsSettings();
        }
        else if (currentState == GameState::SETTINGS_AUDIO)
        {
            ShowAudioSettings();
        }
        else if (currentState == GameState::SETTINGS_CONTROLLER)
        {
            ShowControllerSettings();
        }

        ImGui::EndChild();
        ImGui::End();
    }
}

void ShowGameplaySettings()
{
    ImGui::Text("Gameplay Settings");

    static bool autoSave = true;
    static int difficulty = 2;  // 0: Easy, 1: Normal, 2: Hard

    ImGui::Checkbox("Enable Auto-Save", &autoSave);
    ImGui::SliderInt("Difficulty Level", &difficulty, 0, 2, difficulty == 0 ? "Easy" : difficulty == 1 ? "Normal" : "Hard");
}

void ShowVideoSettings()
{
    ImGui::Text("Video Settings");

    // Fullscreen mode dropdown
    const char* modeItems[] = { "Windowed", "Fullscreen", "Borderless Window" };
    ImGui::Combo("Display Mode", &selectedMode, modeItems, IM_ARRAYSIZE(modeItems));

    // Dynamic resolution dropdown
    if (resolutionList.empty()) {
        GetSupportedResolutions();
    }
    ImGui::Combo("Resolution", &selectedResolution, [](void* data, int idx, const char** out_text)
        {
            *out_text = static_cast<const char*>(resolutionList[idx].c_str());
            return true;
        }, NULL, resolutionList.size());

    // Brightness setting
    static float brightness = 1.0f;
    ImGui::SliderFloat("Brightness", &brightness, 0.0f, 2.0f);

    // Apply button to trigger the changes
    if (ImGui::Button("Apply Changes"))
    {
        // Get selected resolution
        int width = resolutions[selectedResolution].first;
        int height = resolutions[selectedResolution].second;

        // Handle different display modes
        if (selectedMode == 0)  // Windowed
        {
            glfwSetWindowMonitor(window, NULL, 100, 100, width, height, 0);
        }
        else if (selectedMode == 1)  // Fullscreen
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
            glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else if (selectedMode == 2)  // Borderless Window
        {
            glfwSetWindowMonitor(window, NULL, 0, 0, width, height, 0);
            glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
        }

        // Reset window decoration if switching back to windowed
        if (selectedMode == 0)
        {
            glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
        }
    }
}


void ShowGraphicsSettings()
{
    ImGui::Text("Graphics Settings");

    static int textureQuality = 2;  // 0: Low, 1: Medium, 2: High
    static int shadowQuality = 1;   // 0: Off, 1: Low, 2: High
    static bool antiAliasing = true;

    ImGui::Combo("Texture Quality", &textureQuality, "Low\0Medium\0High\0");
    ImGui::Combo("Shadow Quality", &shadowQuality, "Off\0Low\0High\0");
    ImGui::Checkbox("Enable Anti-Aliasing", &antiAliasing);
}

void ShowAudioSettings()
{
    ImGui::Text("Audio Settings");

    static float masterVolume = 1.0f;
    static float musicVolume = 0.5f;
    static float sfxVolume = 0.75f;
    static bool muteAll = false;

    ImGui::SliderFloat("Master Volume", &masterVolume, 0.0f, 1.0f);
    ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 1.0f);
    ImGui::SliderFloat("SFX Volume", &sfxVolume, 0.0f, 1.0f);
    ImGui::Checkbox("Mute All", &muteAll);
}

void ShowControllerSettings()
{
    ImGui::Text("Controller Settings");

    static float sensitivityX = 1.0f;
    static float sensitivityY = 1.0f;
    static bool invertYAxis = false;

    ImGui::SliderFloat("X-Axis Sensitivity", &sensitivityX, 0.1f, 10.0f);
    ImGui::SliderFloat("Y-Axis Sensitivity", &sensitivityY, 0.1f, 10.0f);
    ImGui::Checkbox("Invert Y-Axis", &invertYAxis);
}
