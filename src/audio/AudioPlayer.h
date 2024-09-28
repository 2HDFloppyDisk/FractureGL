#pragma once

#include <string>
#include <vector>

class AudioPlayer {
public:
    AudioPlayer();

    void ShowPlayerUI();           // Renders the player UI
    void LoadEmbeddedAudio(const std::string& audioID);      // Loads the embedded audio track
    void UpdateVisualizer();       // Updates visualizer data
    void SetTrackTime(float time); // Sets track time for display

    void SetCurrentAudio(const std::string& audioID);  // Method to set the current audio ID
    void PlayCurrentAudio();  // Method to play the current audio
    void StopAudio();  // Method to stop the current audio

private:
    std::string currentAudioID;  // Variable to store the currently selected audio ID

    static constexpr int numBands = 20;  // Number of bands in the spectrum
    float spectrum[numBands] = { 0.0f };  // Array to store spectrum data

    float volume;
    bool isMuted;
    float trackTime;
    float trackLength;
    std::string currentTrack;
    std::vector<float> visualizerData;

    void ToggleMute();
    void UpdateSeekBar();

    bool isPlaying = false;  // Tracks if audio is currently playing
};
