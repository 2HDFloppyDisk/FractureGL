#pragma once

#include <string>
#include <vector>

class AudioPlayer {
public:
    AudioPlayer();

    void ShowPlayerUI();           // Renders the player UI
    void LoadEmbeddedAudio(const std::string& audioID);      // Loads the embedded audio track
    void SetTrackTime(float time); // Sets track time for display

    void SetCurrentAudio(const std::string& audioID);  // Method to set the current audio ID
    void PlayCurrentAudio();  // Method to play the current audio
    void StopAudio();  // Method to stop the current audio

    void RenderAudioSelectionUI();  // New method for rendering the audio selection UI

    bool IsPlayerUIVisible() const { return isPlayerUIVisible; }

    void SetVolumeLevel(float newVolume);

    void AddExternalAudio(const std::string& filePath);
    void RemoveExternalAudio(size_t index);

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

    bool isPlayerUIVisible = false;

    bool isPlayingExternal = false;  // Tracks if the currently selected audio is an external file

    // Index of the currently selected audio file
    int currentAudioIndex = -1; // Initialize to -1 to indicate no selection
    std::vector<std::string> embeddedAudioList;
    std::string GetFileName(const std::string& filePath);
    std::vector<std::string> externalAudioTitles;
    std::vector<std::string> externalAudioLengths;
    std::vector<std::string> audioLibrary;  // New member to store the list of external audio files
    std::string OpenFileDialog();
};
