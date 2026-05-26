#pragma once

#include <imgui.h>

#include "UI/UIPanel.hpp"

namespace FWE::UI {
    class UIHelper {
    public:
        static ImVec2 getCenter(ImVec2 windowSize, ImVec2 panelSize) {
            ImVec2 center = ImVec2((windowSize.x - panelSize.x) / 2, (windowSize.y - panelSize.y) / 2);
            return center;
        }

        UIPanel createPanel(std::string id, ImVec2 size, ImVec2 pos, std::function<void()> render, ImGuiWindowFlags flags = 0) {
            return UIPanel(id, size, pos, render);
        }
    };
}