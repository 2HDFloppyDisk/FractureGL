#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

// Include necessary headers
#include <GLFW/glfw3.h>

// Declare the configFilePath as extern to prevent multiple definitions
extern const char* configFilePath;

// Function declarations
void SaveSettingsToConfig(bool isAudioUIVisible, float volumeLevel);
bool LoadSettingsFromConfig(bool& isAudioUIVisible, float& volumeLevel);
void SaveWindowPositionToConfig(GLFWwindow* window);
bool LoadWindowPositionFromConfig(int& posX, int& posY);

#endif  // CONFIG_MANAGER_H
