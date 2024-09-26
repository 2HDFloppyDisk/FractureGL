#include <GLFW/glfw3.h>
#include "ui/MainMenu.h"
#include "ui/Credits.h"
#include "ui/Settings.h"
#include "gameplay/GameStateHandler.h"

extern GameState currentState;


// Function to handle the game state and render the appropriate UI
void HandleGameState() {
    if (currentState == GameState::MAIN_MENU) {
        ShowMainMenu();  // Render the main menu
    }
    else if (currentState == GameState::PLAYING) {

    }
    else if (currentState == GameState::SETTINGS ||
        currentState == GameState::SETTINGS_GAMEPLAY ||
        currentState == GameState::SETTINGS_VIDEO ||
        currentState == GameState::SETTINGS_GRAPHICS ||
        currentState == GameState::SETTINGS_AUDIO ||
        currentState == GameState::SETTINGS_CONTROLLER) {
        ShowSettingsMenu();  // Render the settings menu
    }
    else if (currentState == GameState::CREDITS) {
        ShowCredits();  // Render the credits screen
    }
}
