#include <imgui/imgui.h>
#include "ui/GUIStyle.h"


void ApplySWGStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Set up colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.14f, 0.16f, 0.50f); // Dark background
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.05f, 0.08f, 1.00f); // Title background
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.08f, 0.12f, 1.00f); // Active title background
    colors[ImGuiCol_Border] = ImVec4(0.44f, 0.44f, 0.50f, 0.50f); // Borders with metallic effect
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f); // Frame background
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f); // Hovered frame background
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.35f, 0.39f, 1.00f); // Active frame background
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.23f, 0.26f, 1.00f); // Button color
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.35f, 0.39f, 1.00f); // Hovered button
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f); // Active button
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.23f, 0.26f, 1.00f); // Header background
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.29f, 0.33f, 1.00f); // Hovered header
    colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.35f, 0.39f, 1.00f); // Active header
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.50f, 0.75f, 1.00f); // Checkbox mark color

    // Style settings
    style.WindowRounding = 0.0f; // No window corner rounding
    style.FrameRounding = 2.0f; // Slight rounding for frames
    style.ScrollbarSize = 15.0f; // Larger scrollbar for easier navigation
    style.ScrollbarRounding = 2.0f; // Slight rounding for scrollbar
}
