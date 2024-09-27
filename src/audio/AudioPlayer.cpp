#include "audio/AudioPlayer.h"
#include "audio/FMODManager.h"
#include <imgui/imgui.h>
#include "audio/embedded_audio.h"
#include "utils/LogUtils.h"
#include "ui/GUIStyle.h"

AudioPlayer::AudioPlayer() : volume(0.5f), isMuted(false), trackTime(0.0f), trackLength(0.0f), currentTrack("No Track Loaded"), isPlaying(false) { }

void AudioPlayer::ShowPlayerUI() {
    ApplyWinampStyle(); // Apply custom style

    ImGui::Begin("Winamp Bento Player", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowSize(ImVec2(500, 200)); // Adjust window size

    // First row - Track time and info
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.0f, 0.0f, 0.0f)); // Red background for debugging
    ImGui::BeginChild("FirstRow", ImVec2(0, 40), true);
    ImGui::Columns(2, nullptr, false); // Create two columns for track time and track info
    ImGui::SetColumnWidth(0, 150); // Set the first column's width
    ImGui::Text("- %.2f", trackTime); // Track time
    ImGui::NextColumn();
    ImGui::Text("%s", currentTrack.c_str()); // Track info
    ImGui::EndChild();
    ImGui::PopStyleColor(); // Restore the original style

    // Second row - Visualizer and volume controls
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 1.0f, 0.0f, 0.0f)); // Green background for debugging
    ImGui::BeginChild("SecondRow", ImVec2(0, 40), true);

    ImDrawList* drawList = ImGui::GetWindowDrawList(); // Get draw list for custom rendering

    ImGui::Columns(2, nullptr, false); // Create two columns for visualizer and volume controls
    ImGui::SetColumnWidth(0, 150); // Adjust column width as needed

    // Beat Visualizer
    FMODManager::getInstance().GetSpectrum(spectrum, numBands);

    // Apply style changes specific to the visualizer sliders
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 originalFramePadding = style.FramePadding;
    float originalGrabMinSize = style.GrabMinSize;

    // Set temporary style for the visualizer
    style.FramePadding = ImVec2(0.0f, 0.0f);  // No padding
    style.GrabMinSize = 2.0f;  // Thinner slider grab size
    ImVec4 originalFrameBg = style.Colors[ImGuiCol_FrameBg]; // Save original colors
    ImVec4 originalBorder = style.Colors[ImGuiCol_Border];
    ImVec4 originalGrabColor = style.Colors[ImGuiCol_SliderGrab];
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImVec2 visualizerSize = ImGui::GetContentRegionAvail();
    float barWidth = visualizerSize.x / (numBands * 1.0f);

    for (int i = 0; i < numBands; ++i) {
        if (i > 0) {
            ImGui::SameLine(0, 0);
        }
        ImGui::PushID(i);

        ImVec2 barPosition = ImGui::GetCursorScreenPos();
        ImVec2 barSize(barWidth, 25.0f);

        float fillHeight = spectrum[i] * barSize.y;

        // Adjust the starting point of the fill effect to align with the top of the slider
        drawList->AddRectFilled(
            ImVec2(barPosition.x, barPosition.y + barSize.y - fillHeight),
            ImVec2(barPosition.x + barWidth, barPosition.y + barSize.y),
            ImColor(0, 128, 255, 200)
        );

        ImGui::VSliderFloat("", ImVec2(barWidth, barSize.y), &spectrum[i], 0.0f, 1.0f, "");
        ImGui::PopID();
    }

    // Restore original style settings
    style.FramePadding = originalFramePadding;
    style.GrabMinSize = originalGrabMinSize;
    style.Colors[ImGuiCol_FrameBg] = originalFrameBg;
    style.Colors[ImGuiCol_Border] = originalBorder;
    style.Colors[ImGuiCol_SliderGrab] = originalGrabColor;


    ImGui::NextColumn();
    if (ImGui::Button("Mute")) {
        ToggleMute();
    }
    ImGui::SameLine();
    if (ImGui::SliderFloat("##Volume", &volume, 0.0f, 1.0f, "")) {
        FMODManager::getInstance().SetVolume(volume);
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Third row - Seek bar
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 1.0f, 0.0f));
    ImGui::BeginChild("ThirdRow", ImVec2(0, 35), true);
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 20);
    ImGui::SliderFloat("##seek", &trackTime, 0.0f, trackLength);
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Fourth row - Playback controls
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 0.0f, 0.0f));
    ImGui::BeginChild("FourthRow", ImVec2(0, 40), true);

    if (ImGui::Button("Prev")) {
        FMODManager::getInstance().Previous();
    }
    ImGui::SameLine();
    if (ImGui::Button("Play")) {
        LogToUI("Play button clicked.");
        LoadEmbeddedAudio();
        FMODManager::getInstance().Play();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause")) {
        FMODManager::getInstance().Pause();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        FMODManager::getInstance().Stop();
    }
    ImGui::SameLine();
    if (ImGui::Button("Next")) {
        FMODManager::getInstance().Next();
    }

    ImGui::SameLine(0, 100);
    if (ImGui::Button("Repeat")) {
        // Add repeat functionality
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::End();
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
            currentTrack = "Embedded Audio";
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

void AudioPlayer::ToggleMute() {
    isMuted = !isMuted;
    FMODManager::getInstance().SetMute(isMuted);
}

void AudioPlayer::UpdateSeekBar() {
    // Code to handle seek bar updates
}

void AudioPlayer::UpdateVisualizer() {
    // Placeholder for visualizer update logic
}

void AudioPlayer::SetTrackTime(float time) {
    trackTime = time;
}

