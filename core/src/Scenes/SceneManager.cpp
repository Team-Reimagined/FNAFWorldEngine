#include "Scenes/SceneManager.hpp"

namespace FWE::Scenes
{
    void SceneManager::LoadScene()
    {
        loadQueuedScene = [=, this]()
        {
            currentScene = new Scene(scenePath);
        };
        sceneChanged = true;
    }

    void SceneManager::QueueSceneUnload()
    {
        sceneChanged = true;
        currentScene = nullptr;
    }

    void SceneManager::UnloadCurrentScene()
    {
        if(currentScene != nullptr)
        {
            delete currentScene;
            currentScene = nullptr;
        }
    }
    
    bool SceneManager::Update()
    {
        if(sceneChanged)
        {
            UnloadCurrentScene();
            loadQueuedScene();
            sceneChanged = false;
        }
        if(currentScene == nullptr)
        {
            return false;
        }

        currentScene->Update();
        currentScene->Draw();

        return true;
    }
}