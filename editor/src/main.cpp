#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Renderer/Renderer.hpp"
#include "Input/InputManager.hpp"
#include "MarionetteUI/TextInput.hpp"

int main() {
    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();
    FWE::MarionetteUI::UIManager *uiManager = FWE::MarionetteUI::UIManager::GetInstance();
    FWE::Input::InputManager *inputManager = FWE::Input::InputManager::GetInstance();
    FWE::ResourceLoader::ImageLoader *imgLoader = FWE::ResourceLoader::ImageLoader::GetInstance();

    const bool fixedResolution = false;
    const bool fullscreen = false;

    renderer->Init(fixedResolution, fullscreen);
    uiManager->Init();

    FWE::MarionetteUI::Font font = {uiManager->LoadFont("resources/fonts/fnaf_world_font.ttf"), 32, {0, 255, 255, 255}};

    FWE::Renderer::Image img = imgLoader->LoadImage("resources/Background.png");

    FWE::MarionetteUI::Panel panel({0, 0}, {1, 1}, img, false, FWE::MarionetteUI::HorizontalAlignment::Full, FWE::MarionetteUI::VerticalAlignment::Full);
    FWE::MarionetteUI::TextInput input({0, 0}, {150, 85}, font, "Temp", FWE::MarionetteUI::HorizontalAlignment::Center, FWE::MarionetteUI::VerticalAlignment::Center, 0xA000FFFF);

    uiManager->AddUIElementToTree(&panel);
    uiManager->AddUIElementToTree(&input);
    
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