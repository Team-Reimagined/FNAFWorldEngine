#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace FWE::Nodes
{
    enum Types
    {
        Int,
        Float,
        Bool,
        String,
        Vector2,
        Atlas
    };

    struct RegisteredVariable
    {
        Types type;
        void *variable;
    };

    class Node
    {
    public:
        Node();
        virtual void Init() {}
        virtual void Update() {}
        virtual void Draw() {}
        void AddChild(Node *node);
        glm::vec2 GetGlobalPosition();
        glm::vec2 GetGlobalScale();
    protected:
        void RegisterVariable(const char *name, Types type, void *variable);
    public:
        std::unordered_map<std::string, RegisteredVariable> registeredVariables;
        Node *parent = nullptr;
        std::vector<Node *> children;
        glm::vec2 position = {0, 0};
        glm::vec2 scale = {1, 1};
        std::string name;
    };
}