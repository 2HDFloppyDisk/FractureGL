#include "audio/AudioPlayer.h"
#include "audio/FMODManager.h"
#include <imgui/imgui.h>
#include "audio/embedded_audio.h"
#include "utils/LogUtils.h"
#include "ui/GUIStyle.h"

AudioPlayer::AudioPlayer() : volume(0.5f), currentTrack("No Track Loaded"), isPlaying(false) {}

void AudioPlayer::ShowPlayerUI() {
    // Save the current style
    ImGuiStyle& originalStyle = ImGui::GetStyle();
    ImGuiStyle backupStyle = originalStyle;

    // Apply Bento skin style to the audio player
    ApplyWinampStyle();

    // Begin rendering the audio player with the Bento skin style
    ImGui::Begin("Winamp Bento Player", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    // Track info bar (centered)
    ImGui::Text("Now Playing: %s", currentTrack.c_str());

    // Playback controls (left-aligned)
    ImGui::BeginGroup();
    if (ImGui::Button("Play")) {
        LogToUI("Play button clicked.");
        LoadEmbeddedAudio();
        FMODManager::getInstance().Play();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause")) {
        LogToUI("Pause button clicked.");
        FMODManager::getInstance().Pause();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        LogToUI("Stop button clicked.");
        FMODManager::getInstance().Stop();
    }
    ImGui::EndGroup();

    // Volume slider and equalizer sliders
    ImGui::SameLine();
    ImGui::VSliderFloat("##Volume", ImVec2(20, 100), &volume, 0.0f, 1.0f, "Vol");

    ImGui::SameLine();
    ImGui::BeginChild("Equalizer");
    for (int i = 0; i < 10; ++i) {
        ImGui::VSliderFloat(("##EQ" + std::to_string(i)).c_str(), ImVec2(20, 100), &equalizer[i], 0.0f, 1.0f);
        ImGui::SameLine();
    }
    ImGui::EndChild();

    ImGui::End();

    // Restore the original style
    originalStyle = backupStyle;
}


void AudioPlayer::LoadEmbeddedAudio() {
    FMOD::System* fmodSystem = FMODManager::getInstance().getFMODSystem();

    if (fmodSystem) {
        FMOD_CREATESOUNDEXINFO exInfo = {};
        exInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        exInfo.length = embeddedMP3Size;

        FMOD::Sound* sound = nullptr;
        FMOD_RESULT result = fmodSystem->createSound(reinterpret_cast<const char*>(embeddedMP3), FMOD_OPENMEMORY, &exInfo, &sound);

        if (result == FMOD_OK) {
            FMODManager::getInstance().setSound(sound);
            currentTrack = "Embedded Audio";  // Set a meaningful name for the track
            LogToUI("Embedded audio loaded successfully.");
        }
        else {
            LogToUI("Failed to load embedded audio. FMOD error: " + std::to_string(result));
        }
    }
    else {
        LogToUI("FMOD system not initialized.");
    }
}
