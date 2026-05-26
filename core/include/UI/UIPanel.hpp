#pragma once

#include <functional>
#include <string>

#include <imgui.h>

namespace FWE::UI {
    class UIPanel {
    public:
        const ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

        UIPanel(std::string id, ImVec2 size, ImVec2 pos, std::function<void()> render, ImGuiWindowFlags flags = 0);
    };
}