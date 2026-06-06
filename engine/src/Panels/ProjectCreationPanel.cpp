#include "Panels/ProjectCreationPanel.hpp"

#include "UI/FontManager.hpp"
#include "UI/UIHelper.hpp"

#include "Util/Logging.hpp"

namespace FWE::Panels {
    ProjectCreationPanel::ProjectCreationPanel(ImVec2 pos)
        : UI::UIPanel("ProjectCreationPanel", panelSize, pos, [this]() {
            this->render();
        }, UI::UIHelper::DEFAULT_WINDOW_FLAGS) {
            
        }
    
    void ProjectCreationPanel::render() {
        ImVec2 windowSize = ImGui::GetWindowSize();

        ImVec2 childrenSize = ImVec2(150, 150);
        ImVec2 textSize = ImGui::CalcTextSize("Project Creation");

        ImGui::SetCursorPos(ImVec2((windowSize.x / 2) - textSize.x, (windowSize.y / 2) - textSize.y));
        ImGui::PushFont(NULL, 24.f);
        ImGui::Text("Project Creation");
        ImGui::PopFont();
    }
}