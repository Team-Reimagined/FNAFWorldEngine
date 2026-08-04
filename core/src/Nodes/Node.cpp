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

    Node::~Node()
    {
        for(int i = 0; i < parent->GetChildrenCount(); i++)
        {
            if(parent->GetChild(i) == this)
            {
                parent->RemoveChild(i);
                break;
            }
        }
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

    void Node::AddChild(Node *node)
    {
        if(node->parent == nullptr)
        {
            node->parent = this;
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
        Node *currentNode = parent;
        while (currentNode != nullptr)
        {
            globalPosition += currentNode->position;
            currentNode = currentNode->parent;
        }
        return globalPosition;
    }

    glm::vec2 Node::GetGlobalScale()
    {
        glm::vec2 globalScale = scale;
        Node *currentNode = parent;
        while (currentNode !=nullptr)
        {
            globalScale *= currentNode->scale;
            currentNode = currentNode->parent;
        }
        return globalScale;
    }

    Node *Node::GetParent()
    {
        return parent;
    }

    Node *Node::GetChild(unsigned int index)
    {
        if(index >= children.size())
        {
            return nullptr;
        }
        return children[index];
    }

    std::vector<Node *> &Node::GetChildren()
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
        for(int i = 0; i < parent->GetChildrenCount(); i++)
        {
            if(parent->GetChild(i) == this)
            {
                parent->RemoveChild(i);
                break;
            }
        }
    }
}