#include "EditorPanels/Inspector.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Nodes/Initialize.hpp"
#include "Renderer/Renderer.hpp"
#include "Input/InputManager.hpp"
#include "EditorPanels/SceneTree.hpp"
#include "Scenes/Scene.hpp"

int main() {
    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();
    FWE::MarionetteUI::UIManager *uiManager = FWE::MarionetteUI::UIManager::GetInstance();
    FWE::Input::InputManager *inputManager = FWE::Input::InputManager::GetInstance();
    FWE::ResourceLoader::ImageLoader *imgLoader = FWE::ResourceLoader::ImageLoader::GetInstance();

    const bool fixedResolution = false;
    const bool fullscreen = false;

    renderer->Init(fixedResolution, fullscreen);
    renderer->SetClearColor(0x1A1A1AFF);
    uiManager->Init();

    FWE::Nodes::Initalize();

    FWE::Editor::Inspector inspector;
    uiManager->AddUIElementToTree(&inspector);

    FWE::Scenes::Scene scene("resources/TestScene.scene");
    FWE::Editor::SceneTree sceneTree;
    sceneTree.SetInspector(&inspector);
    uiManager->AddUIElementToTree(&sceneTree);
    sceneTree.SetScene(&scene);
    
    bool running = true;

    while (running)
    {
        SDL_Event events;
        while(SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            default:
                break;
            }
            inputManager->ProcessEvent(&events);
        }
        uiManager->Draw();
        renderer->Render();
    }
    renderer->Shutdown();
}