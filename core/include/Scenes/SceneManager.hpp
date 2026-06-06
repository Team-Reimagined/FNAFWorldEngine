#pragma once

#include "Scene.hpp"
#include <functional>
#include <vector>
#include <string>

namespace FWE::Scenes
{
    class SceneManager
    {
    public:
        void LoadScene(const char *path);
        void QueueSceneUnload();
        bool Update();
        static SceneManager *GetInstance();
        Scene *GetCurrentScene();
    private:
        void UnloadCurrentScene();
    private:
        std::function<void()> loadQueuedScene;
        std::string scenePath = "";
        bool sceneChanged = false;
        Scene *currentScene = nullptr;
        
    };
}