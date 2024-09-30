#ifndef CORE_UI_H
#define CORE_UI_H

#include <GLFW/glfw3.h>

class CoreUI {
public:
    CoreUI(); // Constructor
    ~CoreUI(); // Destructor

    void InitImGui(GLFWwindow* window);
    void RenderImGui();
    void CleanupImGui();
    void DrawMenuBar();
    void UpdateMainWindowSize(GLFWwindow* window);
    void ShowAboutUI(); // Only keep this declaration here.

    // Flags for toggling the visibility of UI components
    bool showAudioSelectionUI;
    bool showDebugConsoleUI;
    bool showAddAudioLibraryUI;
    bool showDelAudioLibraryUI;
    bool showSkinsUI;
    bool showPreferencesUI;
    bool showFeedbackUI;
    bool showAboutmenuUI;

private:
    bool isAboutUIVisible = false;  // Track if the About UI should be shown
};

#endif // CORE_UI_H
