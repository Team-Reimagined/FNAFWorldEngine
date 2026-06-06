#pragma once

#include "UI/UIPanel.hpp"

namespace FWE::Panels {
    class ProjectCreationPanel : public UI::UIPanel {
    public:
        constexpr static const ImVec2 panelSize = ImVec2(640, 480);

        ProjectCreationPanel(ImVec2 pos);

        void render();
    };
}