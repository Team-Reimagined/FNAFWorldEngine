#pragma once

#include "Nodes/Node.hpp"
#include <vector>
#include <memory>
#include <string>

namespace FWE::Scenes
{
    class Scene
    {
    public:
        void Update();
        void Draw();

        Scene(const std::string &scenePath);
        ~Scene();
    private:
        std::vector<std::unique_ptr<Nodes::Node>> nodes;
    };
}