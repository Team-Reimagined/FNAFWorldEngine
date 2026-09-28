#include "EditorPanels/SceneTree.hpp"
#include "EditorPanels/Inspector.hpp"
#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "Nodes/Node.hpp"
#include "SDL3/SDL_clipboard.h"
#include "Scenes/Scene.hpp"
#include "Util/Logging.hpp"
#include "glm/ext/vector_float2.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>

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

    void SceneTree::SetScene(Scenes::Scene *scene)
    {
        this->scene = scene;
        sceneName.SetText(scene->GetName());
    }

    void SceneTree::SetInspector(Inspector *inspector)
    {
        this->inspector = inspector;
    }
    
    static void DrawRecursive(std::shared_ptr<Nodes::Node> currentNode, MarionetteUI::Label &label, int &elementsDrawn, int indentationAmount, float ySeparation, float yOffset)
    {
        const float indentationSize = 4;
        const float xOffset = 10;
        label.position = {++indentationAmount * indentationSize + xOffset, yOffset + ++elementsDrawn * ySeparation};
        label.SetText(currentNode->name.c_str());
        label.Draw();
        for(const auto child : currentNode->GetChildren())
        {
            DrawRecursive(child, label, elementsDrawn, indentationAmount + 1, ySeparation, yOffset);
        }
    }

    void SceneTree::Draw()
    {
        Panel::Draw();
        sceneName.Draw();
        int elementsDrawn = 0;
        for(const auto child : scene->GetRoot()->GetChildren())
        {
            DrawRecursive(child, nodeName, elementsDrawn, 1, ySeparation, yOffset);
        }
    }

    static void FindNodeSelectedRecursive(uint32_t index, uint32_t &currentIndex, Scenes::Scene *scene, std::shared_ptr<Nodes::Node> currentNode, std::shared_ptr<Nodes::Node> &foundNode)
    {
        if(index == currentIndex)
        {
            foundNode = currentNode;
            return;
        }
        for(const auto child : currentNode->GetChildren())
        {
            FindNodeSelectedRecursive(index, ++currentIndex, scene, child, foundNode);
        }
    }

    static std::shared_ptr<Nodes::Node> FindNodeSelected(uint32_t index, Scenes::Scene *scene)
    {
        std::shared_ptr<Nodes::Node> root = scene->GetRoot();
        std::shared_ptr<Nodes::Node> found = nullptr;
        uint32_t currentIndex = 0;
        FindNodeSelectedRecursive(index, currentIndex, scene, root, found);
        return found;
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
        if(elementSelected > rootIndex)
        {
            selectedNode = FindNodeSelected(elementSelected, scene);
            inspector->SetNodeInspected(selectedNode.lock());
        }
        else
        {
            selectedNode.reset();
            inspector->SetNodeInspected();
        }
    }

    void SceneTree::OnRightClick()
    {
        const int rootIndex = 0;
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int elementSelected = (mouseY - yOffset) / ySeparation;
        selectedNode = FindNodeSelected(elementSelected, scene);
        if(!selectedNode.expired())
        {
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
        if(auto selected = selectedNode.lock())
        {
            SDL_SetClipboardText(selected->name.c_str());
        }
    }

    void SceneTree::AddNode()
    {
        Util::Logging::info("New node");
    }

    void SceneTree::DeleteNode()
    {
        if(auto selected = selectedNode.lock())
        {
            selected->RemoveFromTree();
        }
    }
}