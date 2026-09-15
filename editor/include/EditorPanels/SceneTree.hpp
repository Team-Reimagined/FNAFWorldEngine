#pragma once

#include "EditorPanels/Inspector.hpp"
#include "EditorPanels/RightClickPanel.hpp"
#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Node.hpp"
#include "Scenes/Scene.hpp"
#include <vector>

namespace FWE::Editor
{
    struct Elements
    {
        Nodes::Node *node;
        MarionetteUI::Label nameLabel;
    };

    class SceneTree : public MarionetteUI::Panel
    {
    public:
        SceneTree();
        void Draw() override;
        void SetScene(Scenes::Scene *scene);
        void OnLeftClick() override;
        void OnRightClick() override;
        void SetInspector(Inspector *inspector);
    private:
        void LoadTree();
        void LoadTreeRecursive(Nodes::Node *node, int indentationAmount);
    private:
        MarionetteUI::Label sceneName {{10, 10}, "", {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 20}};
        std::vector<Elements> elements;
        Scenes::Scene *scene = nullptr;
        const float yOffset = 15;
        const float ySeparation = 20;
        Inspector *inspector = nullptr;
        FWE::Editor::RightClickPanel rootRightClickPanel {{150, 50}, {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 16}, 0x181818FF};
        FWE::Editor::RightClickPanel nodeRightClickPanel {{150, 50}, {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 16}, 0x181818FF};
        Nodes::Node *selectedNode = nullptr;
    };
}