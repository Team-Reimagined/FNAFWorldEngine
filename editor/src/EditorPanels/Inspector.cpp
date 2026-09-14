#include "EditorPanels/Inspector.hpp"
#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "Nodes/Node.hpp"

namespace FWE::Editor
{
    Inspector::Inspector()
    {
        horizontalAlignment = MarionetteUI::HorizontalAlignment::Right;
        verticalAlignnment = MarionetteUI::VerticalAlignment::Full;
        panel.MakeInternal(this);
        panel.AddChild(&noSelectionText);
        noSelectionText.topLevel = false;
        panel.AddChild(&selectedLabel);
        selectedLabel.topLevel = false;
    }

    void Inspector::Draw()
    {
        panel.Draw();
        if(selectedNode == nullptr)
        {
            noSelectionText.Draw();
        }
        else
        {
            selectedLabel.Draw();
        }
    }

    void Inspector::SetNodeInspected(Nodes::Node *node)
    {
        selectedNode = node;
        if(node == nullptr)
        {
            return;
        }
        selectedLabel.SetText(node->name.c_str());
    }
}