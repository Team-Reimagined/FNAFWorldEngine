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
        loadQueuedScene = [=, this]()
        {
            currentScene.Load(scenePath.c_str());
        };
        sceneChanged = true;
    }

    Scene *SceneManager::GetCurrentScene()
    {
        return &currentScene;
    }

    void SceneManager::QueueSceneUnload()
    {
        sceneChanged = true;
        loadQueuedScene = [](){};
    }

    void SceneManager::UnloadCurrentScene()
    {
        currentScene.Unload();
    }
    
    bool SceneManager::Update()
    {
        if(sceneChanged)
        {
            UnloadCurrentScene();
            loadQueuedScene();
            sceneChanged = false;
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