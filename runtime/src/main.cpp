#include "Renderer/Renderer.hpp"
#include "Scenes/SceneManager.hpp"
#include "Nodes/Initialize.hpp"
#include <thread>
#include <chrono>
#include <string>
#include "External/json.hpp"
#include "Input/InputManager.hpp"

int main() {
    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();
    FWE::Scenes::SceneManager *sceneManager = FWE::Scenes::SceneManager::GetInstance();
    FWE::Input::InputManager *inputManager = FWE::Input::InputManager::GetInstance();

    const bool fixedResolution = true;
    const bool fullscreen = true;
    
    renderer->Init(fixedResolution, fullscreen);
    FWE::Nodes::Initalize();
    std::ifstream runtimeConfigFile("resources/runtime.json");
    nlohmann::json runtimeConfig = nlohmann::json::parse(runtimeConfigFile);
    std::string mainScene = runtimeConfig.at("MainScene");
    sceneManager->LoadScene(mainScene.c_str());

    bool running = true;
    while (running)
    {
        auto startTime = std::chrono::steady_clock::now();
        inputManager->ClearBuffers();
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            inputManager->ProcessEvent(&event);
        }

        if(!sceneManager->Update())
        {
            running = false;
        }
        renderer->Render();
        const std::chrono::duration<double> frameTime(1./60.);
        std::this_thread::sleep_until(startTime + frameTime);
    }
    renderer->Shutdown();
}