#pragma once

#include "EditorPanels/Inspector.hpp"
#include "EditorPanels/RightClickPanel.hpp"
#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Node.hpp"
#include "Scenes/Scene.hpp"
#include <memory>

namespace FWE::Editor
{
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
        void Rename();
        void CopyName();
        void AddNode();
        void DeleteNode();
    private:
        MarionetteUI::Label sceneName {{10, 10}, "", {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 20}};
        MarionetteUI::Label nodeName {{10, 10}, "", {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 16}};
        Scenes::Scene *scene = nullptr;
        const float yOffset = 15;
        const float ySeparation = 20;
        Inspector *inspector = nullptr;
        FWE::Editor::RightClickPanel rootRightClickPanel {{100, 30}, {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 16}, 0x181818FF};
        FWE::Editor::RightClickPanel nodeRightClickPanel {{100, 30}, {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 16}, 0x181818FF};
        std::weak_ptr<Nodes::Node> selectedNode;
    };
}