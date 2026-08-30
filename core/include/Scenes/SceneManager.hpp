#pragma once

#include "Scene.hpp"
#include <functional>
#include <string>

namespace FWE::Scenes
{
    class SceneManager
    {
    public:
        void LoadScene(const char *path);
        void UnloadScene();
        bool Update();
        static SceneManager *GetInstance();
        Scene *GetCurrentScene();
    private:
        std::function<void()> queuedFunction;
        std::string scenePath = "";
        bool functionQueued = false;
        Scene currentScene;
    };
}