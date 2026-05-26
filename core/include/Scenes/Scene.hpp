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

        Scene(const char *scenePath);
        ~Scene();
    private:
        Nodes::Node *root;
    };
}