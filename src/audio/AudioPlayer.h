#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <string>

class AudioPlayer {
public:
    AudioPlayer();
    void ShowPlayerUI();          // Displays the audio player UI
    void LoadEmbeddedAudio();     // Loads embedded audio

private:
    float volume;                 // Player volume
    std::string currentTrack;     // Current track name
    bool isPlaying;               // Playback state
    float equalizer[10]; // Array to store equalizer values for 10 bands
};

#endif // AUDIO_PLAYER_H
