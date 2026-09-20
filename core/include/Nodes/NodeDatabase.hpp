#pragma once

#include "Node.hpp"
#include <memory>
#include <unordered_map>
#include <string>
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
        requires(std::is_base_of_v<Node, T>)
        {
            database.insert({name, [](){return std::make_shared<T>();}});
        }

        std::shared_ptr<Node> CreateNode(const char *type)
        {
            return database.at(type)();
        }

    private:
        std::unordered_map<std::string, std::function<std::shared_ptr<Node>()>> database;
    };
}