#pragma once

#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Node.hpp"

namespace FWE::Editor
{
    class Inspector : public MarionetteUI::Panel
    {
    public:
        Inspector();
        void Draw() override;
        void SetNodeInspected(std::shared_ptr<Nodes::Node> node = nullptr);
    private:
        MarionetteUI::Label noSelectionText {{0, 0}, "Select a node to see properties here", {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 15}, MarionetteUI::HorizontalAlignment::Center, MarionetteUI::VerticalAlignment::Center};
        std::weak_ptr<Nodes::Node> selectedNode;
        MarionetteUI::Label selectedLabel {{0,10}, "", {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 20}, MarionetteUI::HorizontalAlignment::Center, MarionetteUI::VerticalAlignment::Top};
    };
}