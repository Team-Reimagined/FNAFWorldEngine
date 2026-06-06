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
            currentScene = new Scene(scenePath.c_str());
        };
        sceneChanged = true;
    }

    Scene *SceneManager::GetCurrentScene()
    {
        return currentScene;
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