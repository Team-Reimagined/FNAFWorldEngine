#include "Scenes/Scene.hpp"
#include "External/json.hpp"
#include <fstream>
#include "Nodes/NodeDatabase.hpp"

namespace FWE::Scenes
{
    void LoadChildren(nlohmann::json sceneData, FWE::Nodes::Node *parent)
    {
        FWE::Nodes::NodeDatabase *database = FWE::Nodes::NodeDatabase::GetInstance();
        int childrenCount = sceneData.at("ChildCount");
        for(int i = 0; i < childrenCount; i++)
        {
            std::string type = sceneData.at("Children")[i].at("Type");
            FWE::Nodes::Node *node = database->CreateNode(type.c_str());
            for(auto [str, var] : node->registeredVariables)
            {
                var.SetVariable(sceneData.at("Children")[i].at(str));
            }
            parent->AddChild(node);
            LoadChildren(sceneData.at("Children")[i], node);
        }
    }

    Scene::Scene(const char *scenePath)
    {
        FWE::Nodes::NodeDatabase *database = FWE::Nodes::NodeDatabase::GetInstance();
        std::ifstream sceneFile(scenePath);
        nlohmann::json sceneData = nlohmann::json::parse(sceneFile);
        root = database->CreateNode("Node");
        LoadChildren(sceneData.at("Root"), root);
    }

    void DestroyRecursive(Nodes::Node *node)
    {
        for(int i = 0; i < node->GetChildrenCount(); i++)
        {
            DestroyRecursive(node->GetChild(i));
        }
        delete node;
    }

    Scene::~Scene()
    {
        DestroyRecursive(root);
    }

    Nodes::Node *Scene::GetRoot()
    {
        return root;
    }

    void UpdateRecursive(FWE::Nodes::Node *node)
    {
        node->Update();
        for(int i = 0; i < node->GetChildrenCount(); i++)
        {
            UpdateRecursive(node->GetChild(i));
        }
    }

    void Scene::Update()
    {
        UpdateRecursive(root);
    }

    void DrawRecursive(FWE::Nodes::Node *node)
    {
        if(node->visible)
        {
            node->Draw();
            for(int i = 0; i < node->GetChildrenCount(); i++)
            {
                DrawRecursive(node->GetChild(i));
            }
        }
    }

    void Scene::Draw()
    {
        DrawRecursive(root);  
    }
}