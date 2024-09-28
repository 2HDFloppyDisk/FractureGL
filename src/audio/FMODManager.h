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

    void SetMute(bool mute);
    void SetPlaybackPosition(float position);

    // Set the current sound for playback
    void setSound(FMOD::Sound* newSound);

    // Accessor for the FMOD system
    FMOD::System* getFMODSystem();
    FMOD::Channel* GetChannel();  // Make public
    FMOD::Sound* getSound() const;  // Make public

    // Placeholder for playlist functionality
    void Previous();
    void Next();

    void GetSpectrum(float* spectrum, int numBands);

private:
    FMODManager(); // Private constructor for singleton
    FMODManager(const FMODManager&) = delete; // Delete copy constructor
    FMODManager& operator=(const FMODManager&) = delete; // Delete assignment operator

    // FMOD components
    FMOD::System* fmodSystem;
    FMOD::Sound* sound;
    FMOD::Channel* channel;
    bool isMuted; // Track mute status

    FMOD::DSP* fftDSP;
};

#endif // FMOD_MANAGER_H
