#pragma once

#include <functional>
#include <string>

#include <imgui.h>

namespace FWE::UI {
    class UIPanel {
    public:
        UIPanel(std::string id, ImVec2 size, ImVec2 pos, std::function<void()> render, ImGuiWindowFlags flags = 0);
    };
}