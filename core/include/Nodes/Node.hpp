#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "RegisteredVariable.hpp"

namespace FWE::Nodes
{
    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        Node();
        virtual ~Node() {}
        virtual void Init() {}
        virtual void Update() {}
        virtual void Draw() {}
        void AddChild(std::shared_ptr<Node> &node);
        glm::vec2 GetGlobalPosition();
        glm::vec2 GetGlobalScale();
        std::shared_ptr<Node> GetParent();
        std::shared_ptr<Node> GetChild(unsigned int index);
        std::vector<std::shared_ptr<Node>> &GetChildren();
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
        std::weak_ptr<Node> parent;
        std::vector<std::shared_ptr<Node>> children;
    };
}