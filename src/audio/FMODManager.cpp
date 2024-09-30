#include "audio/FMODManager.h"
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include "utils/LogUtils.h"

FMODManager::FMODManager() : fmodSystem(nullptr), sound(nullptr), channel(nullptr) {}

void FMODManager::init() {
    FMOD_RESULT result = FMOD::System_Create(&fmodSystem);
    if (result != FMOD_OK) {
        LogToUI("FMOD System creation failed.");
        return;
    }

    result = fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        LogToUI("FMOD System initialization failed.");
    }
    else {
        LogToUI("FMOD System initialized successfully.");
    }

    // Create and add FFT DSP to the system
    result = fmodSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &fftDSP);
    if (result == FMOD_OK && fftDSP) {
        fftDSP->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, 1024);  // Set FFT window size
        // Get the master channel group and add the DSP
        FMOD::ChannelGroup* masterGroup = nullptr;
        fmodSystem->getMasterChannelGroup(&masterGroup);
        if (masterGroup) {
            masterGroup->addDSP(0, fftDSP);
        }
    }
}

FMODManager& FMODManager::getInstance() {
    static FMODManager instance;
    return instance;
}

FMOD::System* FMODManager::getFMODSystem() {
    return fmodSystem;
}

void FMODManager::SetMute(bool mute) {
    if (channel) {
        isMuted = mute;
        channel->setMute(isMuted);
        LogToUI(isMuted ? "Audio muted." : "Audio unmuted.");
    }
}

void FMODManager::SetPlaybackPosition(float position) {
    if (channel && sound) {
        unsigned int length = 0;
        sound->getLength(&length, FMOD_TIMEUNIT_MS);
        unsigned int newPosition = static_cast<unsigned int>(position * length);
        channel->setPosition(newPosition, FMOD_TIMEUNIT_MS);
        LogToUI("Playback position set to: " + std::to_string(position * 100) + "%.");
    }
}

void FMODManager::GetSpectrum(float* spectrum, int numBands) {
    if (fftDSP) {
        FMOD_DSP_PARAMETER_FFT* fftData = nullptr;
        fftDSP->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftData, nullptr, nullptr, 0);

        if (fftData && fftData->numchannels > 0) {
            // Copy the data for the first channel only
            int numFFTValues = fftData->length / 2;  // Since the spectrum is symmetrical, we only need half
            for (int i = 0; i < numBands; ++i) {
                int index = static_cast<int>((i / static_cast<float>(numBands)) * numFFTValues);
                float value = fftData->spectrum[0][index];  // Use the first channel's spectrum

                // Apply non-linear scaling to enhance sensitivity
                value = sqrtf(value);  // Example: Square root to exaggerate lower values

                // Optionally, scale the values to increase visual impact
                value *= 3.0f;  // Increase the multiplier for more sensitivity

                // Manually clamp the values between 0.0f and 1.0f
                if (value < 0.0f) {
                    value = 0.0f;
                }
                else if (value > 1.0f) {
                    value = 1.0f;
                }

                spectrum[i] = value;
            }
        }
    }
}

FMOD::Channel* FMODManager::GetChannel() {
    return channel;  // Assuming FMODManager keeps track of the current channel
}

void FMODManager::cleanup() {
    if (sound) {
        sound->release();
        sound = nullptr;
    }
    if (fmodSystem) {
        fmodSystem->close();
        fmodSystem->release();
        fmodSystem = nullptr;
    }
    LogToUI("FMOD System cleaned up.");
}

void FMODManager::Play() {
    if (sound && fmodSystem) {
        FMOD_RESULT result = fmodSystem->playSound(sound, nullptr, false, &channel);
        if (result != FMOD_OK) {
            LogToUI("Failed to play sound.");
        }
        else {
            LogToUI("Sound is playing.");
            // Ensure the channel is unpaused
            channel->setPaused(false);
            // Set the initial volume to ensure it's audible
            channel->setVolume(0.5f);
        }
    }
    else {
        LogToUI("Sound or FMOD system not initialized.");
    }
}


void FMODManager::Pause() {
    if (channel) {
        bool isPaused;
        channel->getPaused(&isPaused);
        channel->setPaused(!isPaused);
        LogToUI(isPaused ? "Sound resumed." : "Sound paused.");
    }
}

void FMODManager::Stop() {
    if (channel) {
        channel->stop();
        LogToUI("Sound stopped.");
    }
}

void FMODManager::SetVolume(float volume) {
    if (channel) {
        channel->setVolume(volume);
        LogToUI("Volume set to " + std::to_string(volume));
    }
}

bool FMODManager::LoadTrack(const std::string& filePath) {
    if (fmodSystem) {
        FMOD_RESULT result = fmodSystem->createSound(filePath.c_str(), FMOD_DEFAULT | FMOD_ACCURATETIME, nullptr, &sound);
        if (result != FMOD_OK) {
            LogToUI("Failed to load track: " + filePath);
            return false;
        }
        LogToUI("Track loaded: " + filePath);
        return true;
    }
    return false;
}

void FMODManager::SetAudioLibrary(const std::vector<std::string>& library) {
    audioLibrary = library;
    currentTrackIndex = 0;  // Reset to the first track
}

void FMODManager::SetCurrentTrackIndex(size_t index) {
    if (index < audioLibrary.size()) {
        currentTrackIndex = index;
    }
}

void FMODManager::setSound(FMOD::Sound* newSound) {
    if (sound) {
        sound->release();
    }
    sound = newSound;
}

FMOD::Sound* FMODManager::getSound() const {
    return sound;
}


std::string FMODManager::GetCurrentTrackName() {
    if (sound) {
        return "Current Track";  // Placeholder for actual track name retrieval
    }
    return "No track loaded.";
}

void FMODManager::Next(AudioPlayer& audioPlayer) {
    LogToUI("Current track index (before Next): " + std::to_string(audioPlayer.currentAudioIndex));

    // Increment the index and wrap around if it exceeds the library size
    audioPlayer.currentAudioIndex = (audioPlayer.currentAudioIndex + 1) % audioPlayer.audioLibrary.size();

    LogToUI("Current track index (after Next): " + std::to_string(audioPlayer.currentAudioIndex));

    // Determine if the current audio is external or embedded
    audioPlayer.isPlayingExternal = (audioPlayer.currentAudioIndex >= audioPlayer.embeddedAudioList.size());

    // Set the new current audio and play it
    audioPlayer.SetCurrentAudio(audioPlayer.audioLibrary[audioPlayer.currentAudioIndex]);
    audioPlayer.PlayCurrentAudio();
}

void FMODManager::Previous(AudioPlayer& audioPlayer) {
    LogToUI("Current track index (before Previous): " + std::to_string(audioPlayer.currentAudioIndex));

    // Decrement the index and wrap around if it goes below 0
    if (--audioPlayer.currentAudioIndex < 0) {
        audioPlayer.currentAudioIndex = audioPlayer.audioLibrary.size() - 1;
    }

    LogToUI("Current track index (after Previous): " + std::to_string(audioPlayer.currentAudioIndex));

    // Determine if the current audio is external or embedded
    audioPlayer.isPlayingExternal = (audioPlayer.currentAudioIndex >= audioPlayer.embeddedAudioList.size());

    // Set the new current audio and play it
    audioPlayer.SetCurrentAudio(audioPlayer.audioLibrary[audioPlayer.currentAudioIndex]);
    audioPlayer.PlayCurrentAudio();
}
