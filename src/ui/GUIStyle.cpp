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

void ApplyWinampStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Color palette based on Bento skin
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);  // Dark background
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);  // Button backgrounds
    style.Colors[ImGuiCol_Button] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);   // Button color
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f); // Button hover color
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);  // Button active color
    style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);  // Border color
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);  // Checkmark color (for sliders/equalizer)
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);  // Slider grab color
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);  // Active slider grab

    // Rounding and padding to mimic Bento skin
    style.FrameRounding = 2.0f;  // Slight rounding for buttons and sliders
    style.WindowRounding = 0.0f;  // No rounding on main window
    style.FramePadding = ImVec2(6, 4);  // Padding within frames
    style.ItemSpacing = ImVec2(8, 4);   // Spacing between items
    style.WindowPadding = ImVec2(8, 8); // Padding around window edges

    // Border and size adjustments
    style.FrameBorderSize = 1.0f;  // Add a slight border around frames
    style.WindowBorderSize = 1.0f; // Border around the window

    // Slider tweaks for equalizer
    style.GrabMinSize = 8.0f;      // Size of the slider grab
    style.GrabRounding = 1.0f;     // Slight rounding on the grab for a modern look
}


