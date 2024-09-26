#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "MainMenu.h"
#include "GameStateHandler.h"
#include "Gameplay.h"
#include <cstdlib>  // Include for rand()

extern GameState currentState;
extern GLFWwindow* window;  // Reference to the GLFW window

void ShowMainMenu()
{
    // Get the current window size
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate the position to center the menu
    ImVec2 windowSize = ImVec2(320, 300);  // Estimate the size of the menu
    ImVec2 windowPos = ImVec2((windowWidth - windowSize.x) * 0.5f, (windowHeight - windowSize.y) * 0.5f);

    // Set the window position and auto-resize flag
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    if (ImGui::Begin("Main Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        ImVec2 buttonSize = ImVec2(160, 40);
        ImVec2 centerPos = ImVec2((windowSize.x - buttonSize.x) * 0.5f, 30);  // Center horizontally, adjust vertically

        ImGui::SetCursorPos(centerPos);
        if (ImGui::Button("NEW GAME", buttonSize))
        {
            currentState = GameState::PLAYING;
            ShowGameplay();  // Pass FMOD system to gameplay
            currentState = GameState::MAIN_MENU;
        }

        centerPos.y += 50;
        ImGui::SetCursorPos(centerPos);
        if (ImGui::Button("LOAD GAME", buttonSize))
        {
            // Implement load game functionality here
        }

        centerPos.y += 50;
        ImGui::SetCursorPos(centerPos);
        if (ImGui::Button("SETTINGS", buttonSize))
        {
            currentState = GameState::SETTINGS;
        }

        centerPos.y += 50;
        ImGui::SetCursorPos(centerPos);
        if (ImGui::Button("CREDITS", buttonSize))
        {
            currentState = GameState::CREDITS ;
        }

        centerPos.y += 50;
        ImGui::SetCursorPos(centerPos);
        if (ImGui::Button("EXIT", buttonSize))
        {
            // Close the application by marking the window for closing
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        ImGui::End();
    }
}
