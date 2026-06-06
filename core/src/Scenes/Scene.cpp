#include "Scenes/Scene.hpp"
#include "External/json.hpp"
#include <fstream>
#include "Nodes/NodeDatabase.hpp"
#include "Types/Atlas.hpp"
#include "ResourceLoader/ImageLoader.hpp"

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
                switch (var.type)
                {
                case FWE::Nodes::Types::Int:
                {
                    int value = sceneData.at("Children")[i].at(str);
                    *(int *)var.variable = value;
                    break;
                }
                case FWE::Nodes::Types::Float:
                {
                    float value = sceneData.at("Children")[i].at(str);
                    *(float *)var.variable = value;
                    break;
                }
                case FWE::Nodes::Types::Bool:
                {
                    bool value = sceneData.at("Children")[i].at(str);
                    *(bool *)var.variable = value;
                    break;
                }
                case FWE::Nodes::Types::String:
                {
                    std::string value = sceneData.at("Children")[i].at(str);
                    *(std::string *)var.variable = value;
                    break;
                }
                case FWE::Nodes::Types::Vector2:
                {
                    glm::vec2 value;
                    value.x = sceneData.at("Children")[i].at(str)[0];
                    value.y = sceneData.at("Children")[i].at(str)[1];
                    *(glm::vec2 *)var.variable = value;
                    break;
                }
                case FWE::Nodes::Types::Atlas:
                {
                    Types::Atlas value;
                    ResourceLoader::ImageLoader *loader = ResourceLoader::ImageLoader::GetInstance();
                    std::string filePath = sceneData.at("Children")[i].at(str)[0];
                    value.img = loader->LoadImage(filePath.c_str());
                    value.x = sceneData.at("Children")[i].at(str)[1];
                    value.y = sceneData.at("Children")[i].at(str)[2];
                    value.width = sceneData.at("Children")[i].at(str)[3];
                    value.height = sceneData.at("Children")[i].at(str)[4];
                    *(Types::Atlas *)var.variable = value;
                    break;
                }
                }
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
        node->Draw();
        for(int i = 0; i < node->GetChildrenCount(); i++)
        {
            DrawRecursive(node->GetChild(i));
        }
    }

    void Scene::Draw()
    {
        DrawRecursive(root);  
    }
}