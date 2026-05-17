#pragma once

#include "Node.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace FWE::Nodes
{
    class NodeDatabase
    {
    public:
        static NodeDatabase *GetInstance();
        template<class T> void Register(const char *name);
    private:
        std::unordered_map<std::string, std::unique_ptr<Node>> database;
    };
}