#include "Scenes/SceneManager.hpp"

namespace FWE::Scenes
{
    SceneManager *SceneManager::GetInstance()
    {
        static SceneManager sceneManager;
        return &sceneManager;
    }

    void SceneManager::LoadScene(const char *path)
    {
        scenePath = path;
        queuedFunction = [=, this]()
        {
            currentScene.Load(scenePath.c_str());
        };
        functionQueued = true;
    }

    Scene *SceneManager::GetCurrentScene()
    {
        return &currentScene;
    }

    void SceneManager::UnloadScene()
    {
        queuedFunction = [=, this]()
        {
            currentScene.Unload();
        };
        functionQueued = true;
    }
    
    bool SceneManager::Update()
    {
        if(functionQueued)
        {
            queuedFunction();
            functionQueued = false;
        }

        if(!currentScene.IsLoaded())
        {
            return false;
        }

        currentScene.Update();
        currentScene.Draw();

        return true;
    }
}