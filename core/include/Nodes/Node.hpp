#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "RegisteredVariable.hpp"

namespace FWE::Nodes
{
    class Node
    {
    public:
        Node();
        ~Node();
        virtual void Init() {}
        virtual void Update() {}
        virtual void Draw() {}
        void AddChild(Node *node);
        glm::vec2 GetGlobalPosition();
        glm::vec2 GetGlobalScale();
        Node *GetParent();
        Node *GetChild(unsigned int index);
        std::vector<Node *> &GetChildren();
        unsigned int GetChildrenCount();
        void RemoveChild(unsigned int index);
        void RemoveFromTree();
    protected:
        void RegisterVariable(const char *name, Types type, void *variable);
    public:
        std::unordered_map<std::string, RegisteredVariable> registeredVariables;
        glm::vec2 position = {0, 0};
        glm::vec2 scale = {1, 1};
        std::string name;
        bool visible = true;
    private:
        Node *parent = nullptr;
        std::vector<Node *> children;
    };
}