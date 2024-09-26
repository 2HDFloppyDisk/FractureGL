#include <imgui/imgui.h>
#include "utils/LogUtils.h"
#include "ui/DebugUI.h"

//std::vector<std::string> debugMessages;

void RenderDebugUI() {
    ImGui::Begin("Debug Console");

    if (ImGui::GetWindowPos().x == 0.0f && ImGui::GetWindowPos().y == 0.0f) {
        debugMessages.push_back("Debug system initialized!");  // Add test message once
    }

    ImGui::SetWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    ImGui::BeginChild("Scrolling", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    // Display all messages stored in the debugMessages vector
    for (const auto& message : debugMessages) {
        ImGui::TextUnformatted(message.c_str());
    }

    // Auto-scroll to the bottom if needed
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    ImGui::End();
}
