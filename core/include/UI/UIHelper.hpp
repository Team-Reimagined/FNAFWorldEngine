#pragma once

#include <imgui.h>

#include "UI/UIPanel.hpp"

namespace FWE::UI {
    class UIHelper {
    public:
        static const ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

        static ImVec2 GetCenter(ImVec2 windowSize, ImVec2 panelSize) {
            ImVec2 center = ImVec2((windowSize.x - panelSize.x) / 2, (windowSize.y - panelSize.y) / 2);
            return center;
        }

        static UIPanel CreatePanel(const std::string id, ImVec2 size, ImVec2 pos, std::function<void()> render, ImGuiWindowFlags flags = 0) {
            return UIPanel(id, size, pos, render, flags);
        }
    };
}