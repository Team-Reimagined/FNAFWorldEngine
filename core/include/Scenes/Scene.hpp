#pragma once

#include "Nodes/Node.hpp"

namespace FWE::Scenes
{
    class Scene
    {
    public:
        void Update();
        void Draw();
        
        void Load(const char *scenePath);
        void Unload();

        bool IsLoaded();

        Nodes::Node *GetRoot();

        Scene() {}
        Scene(const char *scenePath);
        ~Scene();
    private:
        Nodes::Node root;
        bool loaded = false;
    };
}