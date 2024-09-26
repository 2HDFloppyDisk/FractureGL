#ifndef GAMESTATEHANDLER_H
#define GAMESTATEHANDLER_H

// Declare the enum class GameState here
enum class GameState {
    MAIN_MENU,
    PLAYING,
    SETTINGS,
    SETTINGS_GAMEPLAY,
    SETTINGS_VIDEO,
    SETTINGS_GRAPHICS,
    SETTINGS_AUDIO,
    SETTINGS_CONTROLLER,
    CREDITS,
    EXITING
};

// Declare currentState as an extern variable to ensure it's defined elsewhere (in Main.cpp)
extern GameState currentState;

// Declaration of the function to handle game state and UI
void HandleGameState();

#endif // GAMESTATEHANDLER_H