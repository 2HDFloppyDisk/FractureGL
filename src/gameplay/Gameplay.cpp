#include "gameplay/Gameplay.h"
#include "audio/FMODManager.h"
#include "utils/LogUtils.h"
#include "audio/embedded_audio.h"
#include <thread>   // For std::this_thread::sleep_for
#include <chrono>   // For std::chrono::milliseconds

static FMOD::Channel* channel = nullptr;  // Keep the channel accessible to check playback status
static FMOD::Sound* sound = nullptr;  // Make sound static to manage its lifecycle properly

// Custom function to convert FMOD error codes to human-readable strings
const char* FMOD_ErrorString(FMOD_RESULT result) {
    switch (result) {
    case FMOD_OK: return "No errors.";
    case FMOD_ERR_FILE_NOTFOUND: return "File not found.";
    case FMOD_ERR_FORMAT: return "Unsupported format.";
    case FMOD_ERR_INVALID_PARAM: return "Invalid parameter.";
    case FMOD_ERR_INITIALIZATION: return "Initialization error.";
    default: return "Unknown error.";
    }
}

void ShowGameplay() {
    debugMessages.push_back("Starting");

    FMOD::System* fmodSystem = FMODManager::getInstance().getFMODSystem();

    // If sound is already playing, do not restart it
    if (IsSoundPlaying()) {
        debugMessages.push_back("Sound is already playing.");
        return;
    }
    /*
    FMOD_CREATESOUNDEXINFO exinfo = { 0 };
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.length = embeddedMP3Size;  // Size of the embedded audio array

    // Load the sound from memory (from the embedded audio)
    FMOD_RESULT result = fmodSystem->createSound((const char*)embeddedMP3, FMOD_OPENMEMORY | FMOD_DEFAULT, &exinfo, &sound);
    if (result != FMOD_OK) {
        debugMessages.push_back(FMOD_ErrorString(result));  // Custom error message
        return;  // Exit if there's an error
    }
    
    // Play the sound
    result = fmodSystem->playSound(sound, 0, false, &channel);
    if (result != FMOD_OK) {
        debugMessages.push_back(FMOD_ErrorString(result));  // Custom error message
        return;
    }
    */
    debugMessages.push_back("Sound is playing");

    // Set volume to maximum
    if (channel) {
        channel->setVolume(1.0f);  // Set volume to maximum
    }
}

bool IsSoundPlaying() {
    bool isPlaying = false;
    if (channel) {
        channel->isPlaying(&isPlaying);
    }
    return isPlaying;
}

// Cleanup function to be called in the main loop after the sound has finished playing
void CleanupSound() {
    if (sound && !IsSoundPlaying()) {
        sound->release();
        sound = nullptr;
        debugMessages.push_back("Sound released");
    }
}
