#include <string>
#include <vector>
#include "imgui/imgui.h"
#include "../utils/LogUtils.h"

// Global vector to store debug messages for the UI
std::vector<std::string> debugMessages;

void LogToUI(const std::string& message) {
    // Append the new message to the debugMessages vector
    debugMessages.push_back(message);
}
