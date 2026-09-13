#pragma once

#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Node.hpp"
#include "Scenes/Scene.hpp"
#include <vector>

namespace FWE::Editor
{
    class SceneTree : public MarionetteUI::UIElement
    {
    public:
        SceneTree();
        void Draw() override;
        void SetScene(Scenes::Scene *scene);
    private:
        void LoadTree();
        void LoadTreeRecursive(Nodes::Node *node, int indentationAmount);
    private:
        MarionetteUI::Panel panel = MarionetteUI::Panel({0, 0}, {300, 1}, MarionetteUI::HorizontalAlignment::Left, MarionetteUI::VerticalAlignment::Full, 0x101010FF);
        MarionetteUI::Label sceneName = MarionetteUI::Label({10, 10}, "", {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 20});
        std::vector<MarionetteUI::Label> elements;
        Scenes::Scene *scene;
    };
}