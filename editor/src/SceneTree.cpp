#include "SceneTree.hpp"
#include "MarionetteUI/Font.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Node.hpp"
#include "Scenes/Scene.hpp"
#include "glm/ext/vector_float2.hpp"

namespace FWE::Editor
{
    SceneTree::SceneTree()
    {
        
    }

    void SceneTree::LoadTreeRecursive(Nodes::Node *node, int indentationAmount)
    {
        const float indentationSize = 8;
        elements.emplace_back(glm::vec2(indentationAmount * indentationSize, 0), node->name.c_str(), MarionetteUI::Font(MarionetteUI::UIManager::GetInstance()->GetDefaultFont(), 16, 0xFFFFFFFF));
        for(unsigned int i = 0; i < node->GetChildrenCount(); i++)
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
        const float yOffset = 15;
        const float ySeparation = 20;
        for(auto &i : elements)
        {
            i.position.y = yOffset + elementsDrawn++ * ySeparation;
            i.Draw();
        }
    }
}