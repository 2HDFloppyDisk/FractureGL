#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "Credits.h"
#include "GameStateHandler.h"

extern GameState currentState;
extern GLFWwindow* window;  // Reference to the GLFW window

// Implement ShowCredits function here
void ShowCredits()
{
    // Get the current window size
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Set the position and size of the credits window
    ImVec2 windowSize = ImVec2(600, 400);
    ImVec2 windowPos = ImVec2((windowWidth - windowSize.x) * 0.5f, (windowHeight - windowSize.y) * 0.5f);

    // Set the window position and size
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    if (ImGui::Begin("Credits", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
    {
        // Display the credits title
        ImGui::Text("FractureGL Credits");
        ImGui::Separator();

        // Get the width of the content region for centering text
        float contentRegionWidth = ImGui::GetContentRegionAvail().x;

        ImGui::Dummy(ImVec2(0.0f, 20.0f));  // Add vertical space (20 pixels)

        // Center "Engine Developed by:" text
        ImGui::SetCursorPosX((contentRegionWidth - ImGui::CalcTextSize("Designed and Developed by:").x) * 0.5f);
        ImGui::Text("Designed and Developed by:");

        // Center bullet point for developer
        ImGui::SetCursorPosX((contentRegionWidth - ImGui::CalcTextSize("• Stephen McCarty").x) * 0.5f);
        ImGui::BulletText("Stephen McCarty");

        ImGui::Dummy(ImVec2(0.0f, 30.0f));  // Add vertical space (20 pixels)

        ImGui::SetCursorPosX((contentRegionWidth - ImGui::CalcTextSize("http://swissarmyknifedev.com | Email: swissarmyknifedev@yahoo.com").x) * 0.5f);
        ImGui::Text("http://swissarmyknifedev.com | Email: swissarmyknifedev@yahoo.com");

        // Calculate button size and position for center-bottom alignment
        ImVec2 buttonSize = ImVec2(220, 40);

        // Get the total height of the current window and subtract the button height + padding
        float buttonPosY = windowSize.y - buttonSize.y - 20.0f;  // 20.0f for padding from the bottom

        // Center the button horizontally and position it at the bottom
        ImGui::SetCursorPosX((windowSize.x - buttonSize.x) * 0.5f);  // Center horizontally
        ImGui::SetCursorPosY(buttonPosY);  // Position at the calculated bottom position

        // Back button to return to main menu
        if (ImGui::Button("Back to Main Menu", buttonSize))
        {
            // Return to main menu state
            currentState = GameState::MAIN_MENU;
        }

        ImGui::End();
    }
}


