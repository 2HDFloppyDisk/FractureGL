#pragma once

#include <string>
#include <vector>

class AudioPlayer {
public:
    AudioPlayer();

    void ShowPlayerUI();           // Renders the player UI
    void LoadEmbeddedAudio();      // Loads the embedded audio track
    void UpdateVisualizer();       // Updates visualizer data
    void SetTrackTime(float time); // Sets track time for display

private:
    static constexpr int numBands = 20;  // Number of bands in the spectrum
    float spectrum[numBands] = { 0.0f };  // Array to store spectrum data

    float volume;
    bool isMuted;
    float trackTime;
    float trackLength;
    std::string currentTrack;
    bool isPlaying;
    std::vector<float> visualizerData;

    void ToggleMute();
    void UpdateSeekBar();
};
