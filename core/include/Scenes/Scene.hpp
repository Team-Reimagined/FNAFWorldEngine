#pragma once

#include "Nodes/Node.hpp"
#include <memory>

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

        std::shared_ptr<Nodes::Node> GetRoot();
        const char *GetName();

        Scene() {}
        Scene(const char *scenePath);
        ~Scene();
    private:
        std::shared_ptr<Nodes::Node> root;
        bool loaded = false;
        std::string name;
    };
}