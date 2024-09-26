#include <GLFW/glfw3.h>
#include "../ui/MainMenu.h"
#include "../ui/Credits.h"
#include "../ui/Settings.h"
#include "../gameplay/GameStateHandler.h"

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
