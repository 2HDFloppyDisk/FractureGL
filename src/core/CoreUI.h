#ifndef CORE_UI_H
#define CORE_UI_H

#include <GLFW/glfw3.h>

// Initializes ImGui with the specified GLFW window
void InitImGui(GLFWwindow* window);

// Renders the ImGui frame, including the menu bar and other UI elements
void RenderImGui();

// Cleans up ImGui resources
void CleanupImGui();

// Draws the menu bar for UI toggles
void DrawMenuBar();

void UpdateMainWindowSize(GLFWwindow* window);

// Flags for toggling the visibility of UI components
extern bool showAudioSelectionUI;
extern bool showDebugConsoleUI;
extern bool showAddAudioLibraryUI;
extern bool showDelAudioLibraryUI;
extern bool showSkinsUI;
extern bool showPreferencesUI;
extern bool showFeedbackUI;
extern bool showAboutUI;

#endif // CORE_UI_H
