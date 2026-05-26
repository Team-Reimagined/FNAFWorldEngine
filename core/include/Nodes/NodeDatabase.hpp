#pragma once

#include "Node.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include "Util/Logging.hpp"
#include <functional>

namespace FWE::Nodes
{
    class NodeDatabase
    {
    public:
        static NodeDatabase *GetInstance()
        {
            static NodeDatabase nodeDatabase;
            return &nodeDatabase;
        }

        template<class T> void Register(const char *name)
        {
            if(std::is_base_of_v<Node, T>)
            {
                database.insert({name, [](){return new T();}});
            }
            else
            {
                Util::Logging::error("Unable to register class. Class is not derived from Node.");
            }
        }

        Node *CreateNode(const char *type)
        {
            return database.at(type)();
        }

    private:
        std::unordered_map<std::string, std::function<FWE::Nodes::Node *()>> database;
    };
}