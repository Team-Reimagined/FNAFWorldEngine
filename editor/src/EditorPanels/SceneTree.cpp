#include "EditorPanels/SceneTree.hpp"
#include "EditorPanels/Inspector.hpp"
#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Node.hpp"
#include "Scenes/Scene.hpp"
#include "Util/Logging.hpp"
#include "glm/ext/vector_float2.hpp"
#include <cstddef>

namespace FWE::Editor
{
    SceneTree::SceneTree()
    {
        horizontalAlignment = MarionetteUI::HorizontalAlignment::Left;
        verticalAlignnment = MarionetteUI::VerticalAlignment::Full;
        size = {panel.size.x, panel.size.y};
        panel.MakeInternal(this);
        sceneName.MakeInternal(this);
    }

    void SceneTree::LoadTreeRecursive(Nodes::Node *node, int indentationAmount)
    {
        const float indentationSize = 8;
        const float fontSize = 16;
        elements.emplace_back(node, MarionetteUI::Label({indentationAmount * indentationSize, 0}, node->name.c_str(), {MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), fontSize, 0xFFFFFFFF}));
        for(size_t i = 0; i < node->GetChildrenCount(); i++)
        {
            LoadTreeRecursive(node->GetChild(i), indentationAmount + 1);
        }
    }

    void SceneTree::LoadTree()
    {
        Nodes::Node *root = scene->GetRoot();
        LoadTreeRecursive(root, 1);
    }

    void SceneTree::SetScene(Scenes::Scene *scene)
    {
        elements.clear();
        this->scene = scene;
        sceneName.SetText(scene->GetName());
        LoadTree();
    }


    void SceneTree::Draw()
    {
        panel.Draw();
        sceneName.Draw();
        int elementsDrawn = 0;
        for(auto &i : elements)
        {
            i.nameLabel.position.y = yOffset + elementsDrawn++ * ySeparation;
            i.nameLabel.Draw();
        }
    }

    void SceneTree::OnLeftClick()
    {
        if(inspector == nullptr)
        {
            Util::Logging::error("Inspector not set in SceneTree");
            return;
        }
        float mouseY;
        SDL_GetMouseState(NULL, &mouseY);
        int elementSelected = (mouseY - yOffset) / ySeparation;
        if(elementSelected > 0 && elementSelected < elements.size())
        {
            inspector->SetNodeInspected(elements[elementSelected].node);
        }
        else
        {
            inspector->SetNodeInspected();
        }
    }

    void SceneTree::SetInspector(Inspector *inspector)
    {
        this->inspector = inspector;
    }
}