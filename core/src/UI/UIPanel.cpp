#include "UI/UIPanel.hpp"

namespace FWE::UI {
    UIPanel::UIPanel(std::string id, ImVec2 size, ImVec2 pos, std::function<void()> render, ImGuiWindowFlags flags) {
        ImGui::Begin(id.c_str(), nullptr, flags);
        ImGui::SetWindowSize(size);
        ImGui::SetWindowPos(pos, ImGuiCond_Once);

        render();

        ImGui::End();
    }
}