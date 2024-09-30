#include "audio/AudioPlayer.h"
#include "audio/FMODManager.h"
#include <imgui/imgui.h>
#include "audio/embedded_audio.h"
#include "utils/LogUtils.h"
#include "ui/GUIStyle.h"
#include "managers/ConfigManager.h"
#include <fstream>
#include <windows.h>
#include <commdlg.h>
#include <filesystem>

extern const char* configFilePath;


AudioPlayer::AudioPlayer() : volume(0.5f), isMuted(false), trackTime(0.0f), trackLength(0.0f), currentTrack("No Track Loaded"), isPlaying(false) {
    // Initialize embedded audio data
    static const std::vector<std::pair<std::string, std::string>> embeddedAudioList = {
        {"Diatribe by Oliver Michael - Parhelion", "2:29"},
        {"Once and for All by The Robbery Continues", "3:43"},
        {"El Rio Fluye by El Rio Fluye", "3:29"}
    };

    for (const auto& audio : embeddedAudioList) {
        audioLibrary.push_back(audio.first);        // Add to the audio library
        externalAudioTitles.push_back(audio.first); // Use the embedded audio title as-is
        externalAudioLengths.push_back(audio.second); // Set the manually specified length
    }
}

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
    static const std::vector<std::string> embeddedAudioList = {
        "Diatribe by Oliver Michael - Parhelion",
        "Once and for All by The Robbery Continues",
        "El Rio Fluye by El Rio Fluye"
    };

    ImVec2 mainWindowSize = ImGui::GetWindowSize();

    ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 227));

    ImGui::SetWindowSize(ImVec2(500, 200));
    ImGui::SetWindowPos(ImVec2(0, 350));
    ImGui::Begin("Audio Selection", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Set up table with 2 columns: "Title" and "Time"
    if (ImGui::BeginTable("AudioListTable", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 50.0f);
        ImGui::TableHeadersRow();

        // Iterate through the audio library and populate table rows
        for (size_t i = 0; i < audioLibrary.size(); ++i) {
            ImGui::TableNextRow();

            // Title column
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Selectable(externalAudioTitles[i].c_str(), currentAudioIndex == i)) {
                // Handle selection
                currentAudioIndex = i;
                SetCurrentAudio(audioLibrary[i]);

                // Determine if the selected audio is external or embedded
                isPlayingExternal = (i >= embeddedAudioList.size());
            }

            // Time column
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", externalAudioLengths[i].c_str());
        }

        ImGui::EndTable();
    }

    // Button to add external audio
    if (ImGui::Button("+")) {
        std::string filePath = OpenFileDialog();
        if (!filePath.empty()) {
            AddExternalAudio(filePath);
        }
    }
    ImGui::SameLine();
    // Button to remove selected audio (only if it's an external file)
    if (ImGui::Button("-")) {
        // Check if the currently selected audio is an external file
        if (isPlayingExternal && currentAudioIndex >= embeddedAudioList.size()) {
            RemoveExternalAudio(currentAudioIndex);
        }
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
                // Set the sound in FMODManager
                FMODManager::getInstance().setSound(sound);
                currentTrack = audioID;  // Set current track to the selected audio ID

                // Get the length of the embedded audio track
                unsigned int length = 0;
                sound->getLength(&length, FMOD_TIMEUNIT_MS); // Get length in milliseconds

                // Convert length to mm:ss format
                unsigned int minutes = (length / 1000) / 60;
                unsigned int seconds = (length / 1000) % 60;
                char lengthString[6];
                sprintf_s(lengthString, sizeof(lengthString), "%d:%02d", minutes, seconds);

                // Update the length in the externalAudioLengths vector
                auto it = std::find(audioLibrary.begin(), audioLibrary.end(), audioID);
                if (it != audioLibrary.end()) {
                    size_t index = std::distance(audioLibrary.begin(), it);
                    externalAudioLengths[index] = lengthString;
                }

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

    // Load and play the selected audio
    if (!currentAudioID.empty()) {
        if (isPlayingExternal) {
            // Use the existing LoadTrack method to load external audio
            if (!FMODManager::getInstance().LoadTrack(currentAudioID)) {
                LogToUI("Failed to load external audio: " + currentAudioID);
                return;
            }

            // Extract the file name from the path
            currentTrack = std::filesystem::path(currentAudioID).stem().string();
        }
        else {
            // Load embedded audio
            LoadEmbeddedAudio(currentAudioID);
            currentTrack = currentAudioID;  // Assuming embedded audio ID is a suitable track name
        }

        FMODManager::getInstance().Play();
        isPlaying = true;  // Update the state to indicate audio is playing
        LogToUI(currentAudioID + " is now playing.");
    }
    else {
        LogToUI("No audio selected.");
    }
}




void AudioPlayer::StopAudio() {
    // Stop the audio using the FMOD manager
    FMODManager::getInstance().Stop();

    // Reset the playback state
    isPlaying = false;
    trackTime = 0.0f;  // Reset track time
    currentTrack = "No Track Loaded";

    // Clear the spectrum data to reset the visualizer
    for (int i = 0; i < numBands; ++i) {
        spectrum[i] = 0.0f;
    }

    // Log the action to the UI
    LogToUI("Audio stopped and visualizer reset.");
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

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(barPos.x, barPos.y + offsetY),
        ImVec2(barPos.x + (barSize.x * adjustedSeekBarValue), barPos.y + offsetY + barSize.y),
        IM_COL32(100, 200, 250, 255) // Fully opaque fill
    );

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



void AudioPlayer::SetTrackTime(float time) {
    trackTime = time;
}

// Function to set the volume level
void AudioPlayer::SetVolumeLevel(float newVolume) {
    volume = newVolume;

    // Your existing code to adjust the volume...
    FMODManager::getInstance().SetVolume(newVolume);

    // Save the state
    SaveSettingsToConfig(isPlayerUIVisible, volume);
}

void AudioPlayer::AddExternalAudio(const std::string& filePath) {
    // Add the file path to the audio library
    audioLibrary.push_back(filePath);

    // Extract file name for display (without extension)
    std::string fileName = GetFileName(filePath);
    externalAudioTitles.push_back(fileName);  // Store the formatted title

    // Load the audio file to get its length
    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = FMODManager::getInstance().getFMODSystem()->createSound(filePath.c_str(), FMOD_DEFAULT | FMOD_ACCURATETIME, nullptr, &sound);
    if (result == FMOD_OK) {
        unsigned int length = 0;
        sound->getLength(&length, FMOD_TIMEUNIT_MS); // Get length in milliseconds

        // Convert length to mm:ss format
        unsigned int minutes = (length / 1000) / 60;
        unsigned int seconds = (length / 1000) % 60;
        char lengthString[6];
        sprintf_s(lengthString, sizeof(lengthString), "%d:%02d", minutes, seconds);
        externalAudioLengths.push_back(lengthString);

        // Release the sound to free memory
        sound->release();
    }
    else {
        externalAudioLengths.push_back("--:--"); // Use placeholder if loading failed
    }
}

void AudioPlayer::RemoveExternalAudio(size_t index) {
    // Calculate the index relative to external files
    size_t externalIndex = index - embeddedAudioList.size();

    // Check if the currently playing audio is the one being removed
    if (isPlayingExternal && currentAudioIndex == index) {
        // Stop playback
        StopAudio();

        // Clear current track info
        currentTrack = "No Track Loaded";
        currentAudioID.clear();
        isPlaying = false;
        isPlayingExternal = false; // Reset external audio flag
    }

    // Remove from the vectors storing external audio information
    audioLibrary.erase(audioLibrary.begin() + externalIndex);
    externalAudioTitles.erase(externalAudioTitles.begin() + externalIndex);
    externalAudioLengths.erase(externalAudioLengths.begin() + externalIndex);

    // Reset current audio index to prevent invalid selection
    currentAudioIndex = -1;
}




std::string AudioPlayer::OpenFileDialog() {
    // Initialize an OPENFILENAME structure
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;  // Owner window (can be set to a handle if needed)
    ofn.lpstrFile = fileName;  // Buffer to store the file name
    ofn.nMaxFile = MAX_PATH;  // Maximum file path length
    ofn.lpstrFilter = L"Audio Files\0*.MP3;*.WAV\0All Files\0*.*\0";  // File type filters
    ofn.nFilterIndex = 1;  // Index of the filter to use initially
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;  // Flags for dialog behavior

    // Show the file dialog
    if (GetOpenFileNameW(&ofn)) {  // Use GetOpenFileNameW for Unicode
        // Convert wide string to standard string
        std::wstring ws(fileName);
        return std::string(ws.begin(), ws.end());
    }

    // Return an empty string if the dialog was canceled or failed
    return "";
}

std::string AudioPlayer::GetFileName(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("/\\");
    std::string fileName = (lastSlash == std::string::npos) ? filePath : filePath.substr(lastSlash + 1);

    size_t lastDot = fileName.find_last_of('.');
    if (lastDot != std::string::npos) {
        fileName = fileName.substr(0, lastDot);
    }

    return fileName;
}