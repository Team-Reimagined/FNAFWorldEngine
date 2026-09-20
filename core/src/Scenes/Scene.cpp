#include "Scenes/Scene.hpp"
#include "External/json.hpp"
#include <fstream>
#include <memory>
#include "Nodes/Node.hpp"
#include "Nodes/NodeDatabase.hpp"

namespace FWE::Scenes
{
    void LoadChildren(nlohmann::json sceneData, std::shared_ptr<FWE::Nodes::Node> &parent)
    {
        FWE::Nodes::NodeDatabase *database = FWE::Nodes::NodeDatabase::GetInstance();
        int childrenCount = sceneData.at("ChildCount");
        for(int i = 0; i < childrenCount; i++)
        {
            std::string type = sceneData.at("Children")[i].at("Type");
            std::shared_ptr<Nodes::Node> node = database->CreateNode(type.c_str());
            for(auto [str, var] : node->registeredVariables)
            {
                var.SetVariable(sceneData.at("Children")[i].at(str));
            }
            parent->AddChild(node);
            LoadChildren(sceneData.at("Children")[i], node);
        }
    }

    void Scene::Load(const char *scenePath)
    {
        if(loaded)
        {
            Unload();
        }
        FWE::Nodes::NodeDatabase *database = FWE::Nodes::NodeDatabase::GetInstance();
        std::ifstream sceneFile(scenePath);
        nlohmann::json sceneData = nlohmann::json::parse(sceneFile);
        name = sceneData.at("SceneName");
        root = database->CreateNode("Node");
        LoadChildren(sceneData.at(name), root);
        loaded = true;
    }

    Scene::Scene(const char *scenePath)
    {
        Load(scenePath);
    }

    void Scene::Unload()
    {
        if(loaded)
        {
            root.reset();
            loaded = false;
        }
    }

    bool Scene::IsLoaded()
    {
        return loaded;
    }

    Scene::~Scene()
    {
        Unload();
    }

    std::shared_ptr<Nodes::Node> Scene::GetRoot()
    {
        return root;
    }

    void UpdateRecursive(std::shared_ptr<FWE::Nodes::Node> node)
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

    void DrawRecursive(std::shared_ptr<FWE::Nodes::Node> node)
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

    const char *Scene::GetName()
    {
        return name.c_str();
    }
}