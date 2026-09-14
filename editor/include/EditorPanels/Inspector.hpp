#pragma once

#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Node.hpp"

namespace FWE::Editor
{
    class Inspector : public MarionetteUI::UIElement
    {
    public:
        Inspector();
        void Draw() override;
        void SetNodeInspected(Nodes::Node *node = nullptr);
    private:
        MarionetteUI::Panel panel = MarionetteUI::Panel({0, 0}, {300, 0}, MarionetteUI::HorizontalAlignment::Right, MarionetteUI::VerticalAlignment::Full, 0x101010FF);
        MarionetteUI::Label noSelectionText = MarionetteUI::Label({0, 0}, "Select a node to see properties here", {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 16, 0xFFFFFFFF}, MarionetteUI::HorizontalAlignment::Center, MarionetteUI::VerticalAlignment::Center);
        Nodes::Node *selectedNode = nullptr;
        MarionetteUI::Label selectedLabel = MarionetteUI::Label({0,10}, "", {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 20, 0xFFFFFFFF}, MarionetteUI::HorizontalAlignment::Center, MarionetteUI::VerticalAlignment::Top);
    };
}