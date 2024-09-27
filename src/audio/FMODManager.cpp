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
}

FMODManager& FMODManager::getInstance() {
    static FMODManager instance;
    return instance;
}

FMOD::System* FMODManager::getFMODSystem() {
    return fmodSystem;
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
        FMOD_RESULT result = fmodSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
        if (result != FMOD_OK) {
            LogToUI("Failed to load track: " + filePath);
            return false;
        }
        LogToUI("Track loaded: " + filePath);
        return true;
    }
    return false;
}

void FMODManager::setSound(FMOD::Sound* newSound) {
    if (sound) {
        sound->release();
    }
    sound = newSound;
}

std::string FMODManager::GetCurrentTrackName() {
    if (sound) {
        return "Current Track";  // Placeholder for actual track name retrieval
    }
    return "No track loaded.";
}

void FMODManager::Previous() {
    LogToUI("Previous track.");
}

void FMODManager::Next() {
    LogToUI("Next track.");
}