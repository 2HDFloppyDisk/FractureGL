#ifndef FMOD_MANAGER_H
#define FMOD_MANAGER_H

#include <FMOD/fmod.hpp>
#include <string>

class FMODManager {
public:
    // Singleton access
    static FMODManager& getInstance();

    // Initialize and clean up FMOD system
    void init();
    void cleanup();

    // Audio control functions
    void Play();
    void Pause();
    void Stop();
    void SetVolume(float volume);
    bool LoadTrack(const std::string& filePath);
    std::string GetCurrentTrackName();

    // Set the current sound for playback
    void setSound(FMOD::Sound* newSound);

    // Accessor for the FMOD system
    FMOD::System* getFMODSystem();

    // Placeholder for playlist functionality
    void Previous();
    void Next();

private:
    // Private constructor for singleton pattern
    FMODManager();

    // Disallow copying and assignment
    FMODManager(const FMODManager&) = delete;
    FMODManager& operator=(const FMODManager&) = delete;

    // FMOD components
    FMOD::System* fmodSystem;
    FMOD::Sound* sound;
    FMOD::Channel* channel;
};

#endif // FMOD_MANAGER_H
