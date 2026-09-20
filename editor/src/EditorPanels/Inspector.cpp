#include "EditorPanels/Inspector.hpp"
#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "Nodes/Node.hpp"

namespace FWE::Editor
{
    Inspector::Inspector() :
    MarionetteUI::Panel({0, 0}, {300, 0}, MarionetteUI::HorizontalAlignment::Right, MarionetteUI::VerticalAlignment::Full, 0x101010FF)
    {
        horizontalAlignment = MarionetteUI::HorizontalAlignment::Right;
        verticalAlignnment = MarionetteUI::VerticalAlignment::Full;
        noSelectionText.MakeInternal(this);
        noSelectionText.topLevel = false;
        selectedLabel.MakeInternal(this);
        selectedLabel.topLevel = false;
    }

    void Inspector::Draw()
    {
        Panel::Draw();
        if(selectedNode == nullptr)
        {
            noSelectionText.Draw();
        }
        else
        {
            selectedLabel.Draw();
        }
    }

    void Inspector::SetNodeInspected(std::shared_ptr<Nodes::Node> node)
    {
        selectedNode = node;
        if(node == nullptr)
        {
            return;
        }
        selectedLabel.SetText(node->name.c_str());
    }

    void Inspector::NodeDeleted(std::shared_ptr<Nodes::Node> node)
    {
        if(selectedNode == node)
        {
            selectedNode = nullptr;
        }
    }
}