#include "audio/AudioPlayer.h"
#include "audio/FMODManager.h"
#include <imgui/imgui.h>
#include "audio/embedded_audio.h"
#include "utils/LogUtils.h"
#include "ui/GUIStyle.h"

AudioPlayer::AudioPlayer() : volume(0.5f), isMuted(false), trackTime(0.0f), trackLength(0.0f), currentTrack("No Track Loaded"), isPlaying(false) { }

void AudioPlayer::ShowPlayerUI() {
    isPlayerUIVisible = true;  // Set the UI to visible


    ImGui::SetNextWindowSize(ImVec2(500.0f, 200.0f));
    ImGui::SetNextWindowPos(ImVec2(0, 27)); // Example fixed position

    ApplyWinampStyle(); // Apply custom style

    ImGui::Begin("Audio Player", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    //ImGui::SetWindowSize(ImVec2(500, 200)); // Adjust window size

    // First row - Track time and info
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.0f, 0.0f, 0.0f)); // Red background for debugging
    ImGui::BeginChild("FirstRow", ImVec2(0, 40), true);
    ImGui::Columns(2, nullptr, false); // Create two columns for track time and track info
    ImGui::SetColumnWidth(0, 150); // Set the first column's width
    // Calculate minutes and seconds from trackTime
    int minutes = static_cast<int>(trackTime) / 60;
    int seconds = static_cast<int>(trackTime) % 60;

    // Format the time as mm:ss
    char timeString[6]; // Enough to hold "mm:ss\0"
    sprintf_s(timeString, sizeof(timeString), "%d:%02d", minutes, seconds);

    // Display the formatted time
    ImGui::Text("%s", timeString);

    ImGui::NextColumn();

    

    // Create a child window for scrolling text
    ImGui::BeginChild("TrackInfo", ImVec2(ImGui::GetContentRegionAvail().x, 20), false, ImGuiWindowFlags_NoScrollbar);

    static float scrollPos = 0.0f;       // Scroll position for the text
    static bool scrollLeft = true;       // Direction of scrolling
    static float scrollSpeed = 0.1f;     // Adjust scrolling speed (slower now)
    static float pauseTimer = 0.0f;      // Timer for pausing at both ends

    // Constants for timing
    const float initialDelay = 2.0f;     // Delay before starting the scroll (in seconds)
    const float pauseDuration = 1.0f;    // Pause duration at the end points (in seconds)

    // Get the text size to know when to start scrolling
    ImVec2 textSize = ImGui::CalcTextSize(currentTrack.c_str());
    float windowWidth = ImGui::GetContentRegionAvail().x;  // Available width in the child window

    // Scroll only if the text is wider than the window and the initial delay has passed
    if (trackTime >= initialDelay && textSize.x > windowWidth) {
        // Handle the pause at the end points
        if (pauseTimer > 0.0f) {
            pauseTimer -= ImGui::GetIO().DeltaTime;  // Decrement the pause timer
        }
        else {
            // Scroll the text
            if (scrollLeft) {
                scrollPos -= scrollSpeed;  // Scroll to the left
                if (scrollPos < -(textSize.x - windowWidth)) {
                    scrollLeft = false;  // Change direction to the right
                    pauseTimer = pauseDuration;  // Start the pause timer
                }
            }
            else {
                scrollPos += scrollSpeed;  // Scroll to the right
                if (scrollPos > 0) {
                    scrollLeft = true;  // Change direction to the left
                    pauseTimer = pauseDuration;  // Start the pause timer
                }
            }
        }
    }

    // Use SetCursorPosX to create a scrolling effect
    ImGui::SetCursorPosX(scrollPos);
    ImGui::Text("%s", currentTrack.c_str());

    ImGui::EndChild();






    ImGui::EndChild();
    ImGui::PopStyleColor(); // Restore the original style

    // Second row - Visualizer and volume controls
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 1.0f, 0.0f, 0.0f)); // Green background for debugging
    ImGui::BeginChild("SecondRow", ImVec2(0, 45), true);

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
    
    
    //Volume control
    // Determine the size and position of the volume bar
    ImVec2 volumeBarSize = ImVec2(150.0f, 2.0f); // Width and height of the volume bar
    ImVec2 volumeBarPos = ImGui::GetCursorScreenPos(); // Get the screen position to draw the bar

    // Slow down the fill by scaling down the volume value
    float adjustedVolume = volume * 0.95f; // Adjust the scaling factor as needed to slow down the fill

    // Draw the filled portion of the volume bar first
    float volumeHandleHeight = 20.0f; // Height of the handle
    float volumeOffsetY = (volumeHandleHeight - volumeBarSize.y) * 0.5f; // Offset to center the fill

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(volumeBarPos.x, volumeBarPos.y + volumeOffsetY),
        ImVec2(volumeBarPos.x + (volumeBarSize.x * adjustedVolume), volumeBarPos.y + volumeOffsetY + volumeBarSize.y),
        IM_COL32(100, 200, 250, 255) // Fill color
    );

    // Set cursor for the slider handle and draw it
    ImGui::SetCursorScreenPos(volumeBarPos); // Reset cursor to the start of the volume bar
    ImGui::SetNextItemWidth(volumeBarSize.x); // Set the width of the volume slider

    // Temporarily modify ImGui style for the volume slider
    float originalVolumeGrabMinSize = style.GrabMinSize;  // Save original grab size
    float originalVolumeFrameRounding = style.FrameRounding; // Save original frame rounding
    ImVec4 originalVolumeSliderColor = style.Colors[ImGuiCol_FrameBg]; // Save original slider background color

    // Adjust style for a transparent background and wider slider handle for volume control
    style.GrabMinSize = 20.0f;  // Set the size for the volume handle
    style.FrameRounding = 0.0f;  // No rounding for the slider frame
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0, 0, 0, 0); // Transparent background

    // Draw the volume slider handle
    if (ImGui::SliderFloat("##Volume", &volume, 0.0f, 1.0f, "")) {
        FMODManager::getInstance().SetVolume(volume);
    }

    // Restore the original style for the volume slider
    style.GrabMinSize = originalVolumeGrabMinSize;
    style.FrameRounding = originalVolumeFrameRounding;
    style.Colors[ImGuiCol_FrameBg] = originalVolumeSliderColor;



    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Third row - Seek bar
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 1.0f, 0.0f));
    ImGui::BeginChild("ThirdRow", ImVec2(0, 38), true);
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 20);

    UpdateSeekBar();

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

        // Play the currently selected audio
        PlayCurrentAudio();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause")) {
        FMODManager::getInstance().Pause();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        StopAudio();
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

    // Get the current position and size of the main window
    ImVec2 mainWindowPos = ImGui::GetWindowPos();
    ImVec2 mainWindowSize = ImGui::GetWindowSize();



    ImGui::End();

    isPlayerUIVisible = false;  // Reset visibility if window is closed

    // Set the position for the "Audio Selection" window to appear at the bottom of the main window
    //ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x, mainWindowPos.y + mainWindowSize.y), ImGuiCond_Always);
    //ImGui::SetNextWindowSize(ImVec2(500, 150), ImGuiCond_Appearing);  // Set initial size

}
/*
void AudioPlayer::RenderAudioSelectionUI() {
    // Get the size of the main window to use for other windows
    ImVec2 mainWindowSize = ImGui::GetWindowSize();

    // Set the size and position of the audio selection UI to match the main window
    ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 227)); // Example fixed position

    ImGui::SetWindowSize(ImVec2(500, 200)); // Adjust window size
    ImGui::SetWindowPos(ImVec2(0, 350));
    ImGui::Begin("Audio Selection", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Track Selection
    const char* audioList[] = { "Diatribe by Oliver Michael - Parhelion", "Once and for All by The Robbery Continues", "El Rio Fluye by El Rio Fluye" };
    static int currentAudioIndex = 0;

    ImGui::Combo("Select Audio", &currentAudioIndex, audioList, IM_ARRAYSIZE(audioList));

    // Set the current audio based on the selection
    SetCurrentAudio(audioList[currentAudioIndex]);


    ImGui::End();  // End the audio selection window
}
*/

void AudioPlayer::RenderAudioSelectionUI() {
    // Get the size of the main window to use for other windows
    ImVec2 mainWindowSize = ImGui::GetWindowSize();
    
    // Set the size and position of the audio selection UI to match the main window
    ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 227)); // Example fixed position

    ImGui::SetWindowSize(ImVec2(500, 200)); // Adjust window size
    ImGui::SetWindowPos(ImVec2(0, 350));
    ImGui::Begin("Audio Selection", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    static int currentAudioIndex = 0;
    //ImGui::Begin("Audio Selection", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    // Set up table with 2 columns: "Title" and "Time"
    if (ImGui::BeginTable("AudioListTable", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
        // Set up headers
        ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 50.0f);  // Fixed width for the time column
        ImGui::TableHeadersRow();

        // Sample data: Replace with your actual audio list data
        const char* audioList[] = { "Diatribe by Oliver Michael - Parhelion", "Once and for All by The Robbery Continues", "El Rio Fluye by El Rio Fluye" };

        const char* audioTimes[] = {
            "3:50", "4:14", "3:58"
        };


        // Iterate through audio list and populate table rows
        for (int i = 0; i < IM_ARRAYSIZE(audioList); i++) {
            ImGui::TableNextRow();

            // Title column
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Selectable(audioList[i], i == currentAudioIndex)) {
                // Handle selection
                currentAudioIndex = i;
                SetCurrentAudio(audioList[currentAudioIndex]);
            }

            // Time column
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", audioTimes[i]);
        }

        ImGui::EndTable();
    }

    ImGui::End();  // End the audio selection window
}




void AudioPlayer::LoadEmbeddedAudio(const std::string& audioID) {
    FMOD::System* fmodSystem = FMODManager::getInstance().getFMODSystem();
    if (fmodSystem) {
        // Check if the audio ID exists in the maps
        if (audioFiles.count(audioID) && audioFileSizes.count(audioID)) {
            FMOD_CREATESOUNDEXINFO exInfo = {};
            exInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
            exInfo.length = audioFileSizes[audioID];  // Use the size from the map

            FMOD::Sound* sound = nullptr;
            FMOD_RESULT result = fmodSystem->createSound(reinterpret_cast<const char*>(audioFiles[audioID]), FMOD_OPENMEMORY, &exInfo, &sound);  // Use the audio data from the map

            if (result == FMOD_OK) {
                FMODManager::getInstance().setSound(sound);
                currentTrack = audioID;  // Set current track to the selected audio ID
                LogToUI(audioID + " loaded successfully.");
            }
            else {
                LogToUI("Failed to load " + audioID + ". FMOD error: " + std::to_string(result));
            }
        }
        else {
            LogToUI("Audio ID not found: " + audioID);
        }
    }
    else {
        LogToUI("FMOD system not initialized.");
    }
}

void AudioPlayer::SetCurrentAudio(const std::string& audioID) {
    currentAudioID = audioID;
}

void AudioPlayer::PlayCurrentAudio() {
    // Stop the current audio if it is playing
    if (isPlaying) {
        StopAudio();
    }

    // Load and play the new selected audio
    if (!currentAudioID.empty()) {
        LoadEmbeddedAudio(currentAudioID);
        FMODManager::getInstance().Play();
        isPlaying = true;  // Update the state to indicate audio is playing
        LogToUI(currentAudioID + " is now playing.");
    }
    else {
        LogToUI("No audio selected.");
    }
}


void AudioPlayer::StopAudio() {
    FMODManager::getInstance().Stop();  // Stops the audio using the FMOD manager
    isPlaying = false;  // Reset the state to indicate no audio is playing
    LogToUI("Audio stopped.");
}


void AudioPlayer::ToggleMute() {
    isMuted = !isMuted;
    FMODManager::getInstance().SetMute(isMuted);
}

void AudioPlayer::UpdateSeekBar() {
    // Get the current channel and sound from FMODManager
    FMOD::Channel* channel = FMODManager::getInstance().GetChannel();
    FMOD::Sound* sound = FMODManager::getInstance().getSound();

    if (!channel || !sound) {
        return; // No audio playing
    }

    // Variables to hold playback position and length
    unsigned int currentPosition = 0;
    unsigned int trackLength = 0;

    // Get the current playback position
    channel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);

    // Get the total length of the current audio track
    sound->getLength(&trackLength, FMOD_TIMEUNIT_MS);

    // Update the track time in seconds
    trackTime = static_cast<float>(currentPosition) / 1000.0f;

    // Calculate the seek bar value (normalize it between 0.0 and 1.0)
    float seekBarValue = (trackLength > 0) ? static_cast<float>(currentPosition) / static_cast<float>(trackLength) : 0.0f;

    // Slow down the fill by scaling down the seekBarValue
    float adjustedSeekBarValue = seekBarValue * 0.95f; // Adjust the scaling factor as needed to slow down the fill

    // Determine the size and position of the seek bar
    ImVec2 barSize = ImVec2(ImGui::GetContentRegionAvail().x, 2.0f); // Thinner bar height
    ImVec2 barPos = ImGui::GetCursorScreenPos(); // Get the screen position to draw the bar

    // Draw the filled (played) portion of the seek bar first
    float handleHeight = 20.0f; // Height of the handle
    float offsetY = (handleHeight - barSize.y) * 0.5f; // Offset to center the fill

    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(barPos.x, barPos.y + offsetY),
        ImVec2(barPos.x + (barSize.x * adjustedSeekBarValue), barPos.y + offsetY + barSize.y),
        IM_COL32(100, 200, 250, 255)); // Fully opaque fill

    // Set cursor for the slider handle and draw it
    ImGui::SetCursorScreenPos(barPos); // Reset cursor to the start of the bar
    ImGui::SetNextItemWidth(barSize.x); // Set the width of the slider

    // Temporarily modify ImGui style to remove the background of the slider
    ImGuiStyle& style = ImGui::GetStyle();
    float originalGrabMinSize = style.GrabMinSize;  // Save original grab size
    float originalFrameRounding = style.FrameRounding; // Save original frame rounding
    ImVec4 originalSliderColor = style.Colors[ImGuiCol_FrameBg]; // Save original slider background color

    // Adjust style for a transparent background and wider slider handle
    style.GrabMinSize = 40.0f;  // Increase the handle size for a wider slider
    style.FrameRounding = 0.0f;  // No rounding for the slider frame
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0, 0, 0, 0); // Transparent background

    // Use SliderFloat to draw the interactive handle
    if (ImGui::SliderFloat("##SeekBarHandle", &seekBarValue, 0.0f, 1.0f, "")) {
        // If the user interacts with the slider, set the new playback position
        unsigned int newPosition = static_cast<unsigned int>(seekBarValue * trackLength);
        channel->setPosition(newPosition, FMOD_TIMEUNIT_MS);
    }

    // Restore the original style
    style.GrabMinSize = originalGrabMinSize;
    style.FrameRounding = originalFrameRounding;
    style.Colors[ImGuiCol_FrameBg] = originalSliderColor;
}

void AudioPlayer::UpdateVisualizer() {
    // Placeholder for visualizer update logic
}

void AudioPlayer::SetTrackTime(float time) {
    trackTime = time;
}
