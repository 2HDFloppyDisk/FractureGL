#include "managers/ConfigManager.h"
#include <fstream>
#include <string>

// Path to config file
const char* configFilePath = "config.ini";

// Function to save settings to config.ini
void SaveSettingsToConfig(bool isAudioUIVisible, float volumeLevel) {
    std::ofstream outFile(configFilePath, std::ios::app);  // Open in append mode to add settings
    if (outFile.is_open()) {
        outFile << "[Settings]\n";
        outFile << "AudioUIVisible=" << (isAudioUIVisible ? "true" : "false") << "\n";
        outFile << "VolumeLevel=" << volumeLevel << "\n";
        outFile.close();
    }
}

// Function to load settings from config.ini
bool LoadSettingsFromConfig(bool& isAudioUIVisible, float& volumeLevel) {
    std::ifstream inFile(configFilePath);
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            if (line.find("AudioUIVisible=") != std::string::npos) {
                std::string value = line.substr(line.find('=') + 1);
                isAudioUIVisible = (value == "true");
            }
            else if (line.find("VolumeLevel=") != std::string::npos) {
                volumeLevel = std::stof(line.substr(line.find('=') + 1));
            }
        }
        inFile.close();
        return true;
    }
    return false;
}

// Move the window position functions as well
void SaveWindowPositionToConfig(GLFWwindow* window) {
    int posX, posY;
    glfwGetWindowPos(window, &posX, &posY);

    std::ofstream outFile(configFilePath, std::ios::app);
    if (outFile.is_open()) {
        outFile << "[Window]\n";
        outFile << "PosX=" << posX << "\n";
        outFile << "PosY=" << posY << "\n";
        outFile.close();
    }
}

bool LoadWindowPositionFromConfig(int& posX, int& posY) {
    std::ifstream inFile(configFilePath);
    if (!inFile.is_open()) {
        // Return false if the file does not exist or cannot be opened
        return false;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.find("PosX=") != std::string::npos) {
            posX = std::stoi(line.substr(line.find('=') + 1));
        }
        else if (line.find("PosY=") != std::string::npos) {
            posY = std::stoi(line.substr(line.find('=') + 1));
        }
    }
    inFile.close();
    return true;
}
