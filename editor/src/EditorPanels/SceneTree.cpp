#include "EditorPanels/SceneTree.hpp"
#include "EditorPanels/Inspector.hpp"
#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Node.hpp"
#include "SDL3/SDL_clipboard.h"
#include "Scenes/Scene.hpp"
#include "Util/Logging.hpp"
#include "glm/ext/vector_float2.hpp"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace FWE::Editor
{
    SceneTree::SceneTree() :
    Panel({0, 0}, {300, 0}, MarionetteUI::HorizontalAlignment::Left, MarionetteUI::VerticalAlignment::Full, 0x101010FF)
    {
        sceneName.MakeInternal(this);
        AddChild(&rootRightClickPanel);
        AddChild(&nodeRightClickPanel);

        rootRightClickPanel.AddOption("Rename", [=, this](){Rename();});
        rootRightClickPanel.AddOption("Copy name", [=, this](){CopyName();});
        rootRightClickPanel.AddOption("Add node", [=, this](){AddNode();});
        
        nodeRightClickPanel.AddOption("Rename", [=, this](){Rename();});
        nodeRightClickPanel.AddOption("Copy name", [=, this](){CopyName();});
        nodeRightClickPanel.AddOption("Add node", [=, this](){AddNode();});
        nodeRightClickPanel.AddOption("Delete node", [=, this](){DeleteNode();});
    }

    void SceneTree::LoadTreeRecursive(std::shared_ptr<Nodes::Node> node, int indentationAmount)
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
        std::shared_ptr<Nodes::Node> root = scene->GetRoot();
        LoadTreeRecursive(root, 1);
    }

    void SceneTree::SetScene(Scenes::Scene *scene)
    {
        elements.clear();
        this->scene = scene;
        sceneName.SetText(scene->GetName());
        LoadTree();
    }

    void SceneTree::SetInspector(Inspector *inspector)
    {
        this->inspector = inspector;
    }

    void SceneTree::Draw()
    {
        Panel::Draw();
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
        const int rootIndex = 0;
        if(inspector == nullptr)
        {
            Util::Logging::error("Inspector not set in SceneTree");
            return;
        }
        float mouseY;
        SDL_GetMouseState(NULL, &mouseY);
        int elementSelected = (mouseY - yOffset) / ySeparation;
        if(elementSelected > rootIndex && elementSelected < elements.size())
        {
            inspector->SetNodeInspected(elements[elementSelected].node);
        }
        else
        {
            inspector->SetNodeInspected();
        }
    }

    void SceneTree::OnRightClick()
    {
        const int rootIndex = 0;
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int elementSelected = (mouseY - yOffset) / ySeparation;
        if(elementSelected < elements.size())
        {
            selectedNode = elements[elementSelected].node;
            if(elementSelected == rootIndex)
            {
                rootRightClickPanel.ShowPanel({mouseX, mouseY});
            }
            else
            {
                nodeRightClickPanel.ShowPanel({mouseX, mouseY});
            }
            
        }
    }

    void SceneTree::Rename()
    {
        Util::Logging::info("Rename");
    }

    void SceneTree::CopyName()
    {
        SDL_SetClipboardText(selectedNode->name.c_str());
    }

    void SceneTree::AddNode()
    {
        Util::Logging::info("New node");
    }

    void SceneTree::DeleteNode()
    {
        for(size_t i = 0; i < elements.size(); i++)
        {
            if(elements[i].node == selectedNode)
            {
                std::swap(elements[i], elements.back());
                elements.pop_back();
                inspector->NodeDeleted(selectedNode);
                selectedNode->RemoveFromTree();
                selectedNode = nullptr;
            }
        }
    }
}