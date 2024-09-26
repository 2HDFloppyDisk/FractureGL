#include <GLFW/glfw3.h>
#include "MainMenu.h"
#include "Credits.h"
#include "Settings.h"
#include "GameStateHandler.h"

extern GameState currentState;


// Function to handle the game state and render the appropriate UI
void HandleGameState() {
    if (currentState == GameState::MAIN_MENU) {
        ShowMainMenu();  // Render the main menu
    }
    else if (currentState == GameState::PLAYING) {

    }
    else if (currentState == GameState::SETTINGS) {
        ShowSettingsMenu();  // Render the settings menu
    }
    else if (currentState == GameState::CREDITS) {
        ShowCredits();  // Render the credits screen
    }
}
