#include "Nodes/Node.hpp"
#include "Util/Logging.hpp"

namespace FWE::Nodes
{
    Node::Node()
    {
        RegisterVariable("Name", Types::String, &name);
        RegisterVariable("Position", Types::Vector2, &position);
        RegisterVariable("Scale", Types::Vector2, &scale);
    }

    void Node::RegisterVariable(const char *name, Types type, void *variable)
    {
        if(variable != nullptr)
        {
            RegisteredVariable var = {type, variable};
            registeredVariables.insert({name, var});
        }
        else
        {
            Util::Logging::error("Variable cannot be a nullptr");
        }
    }

    void Node::AddChild(std::shared_ptr<Node> &node)
    {
        if(node->parent.expired())
        {
            node->parent = weak_from_this();
            children.push_back(node);
        }
        else
        {
            Util::Logging::error("Node is already a child of another node");
        }
    }

    glm::vec2 Node::GetGlobalPosition()
    {
        glm::vec2 globalPosition = position;
        std::shared_ptr<Node> currentNode = GetParent();
        while (currentNode != nullptr)
        {
            globalPosition += currentNode->position;
            currentNode = currentNode->GetParent();
        }
        return globalPosition;
    }

    glm::vec2 Node::GetGlobalScale()
    {
        glm::vec2 globalScale = scale;
        std::shared_ptr<Node> currentNode = GetParent();
        while (currentNode != nullptr)
        {
            globalScale *= currentNode->scale;
            currentNode = currentNode->GetParent();
        }
        return globalScale;
    }

    std::shared_ptr<Node> Node::GetParent()
    {
        return parent.lock();
    }

    std::shared_ptr<Node> Node::GetChild(unsigned int index)
    {
        if(index >= children.size())
        {
            return nullptr;
        }
        return children[index];
    }

    std::vector<std::shared_ptr<Node>> &Node::GetChildren()
    {
        return children;
    }

    unsigned int Node::GetChildrenCount()
    {
        return children.size();
    }

    void Node::RemoveChild(unsigned int index)
    {
        if(index >= children.size())
        {
            return;
        }
        children.erase(children.begin() + index);
    }

    void Node::RemoveFromTree()
    {
        if(auto sharedParent = parent.lock())
        {
            for(int i = 0; i < sharedParent->GetChildrenCount(); i++)
            {
                if(sharedParent->GetChild(i) == shared_from_this())
                {
                    sharedParent->RemoveChild(i);
                    break;
                }
            }
        }
    }
}