#include "Renderer/Renderer.hpp"
#include "Audio/AudioManager.hpp"
#include <thread>
#include <chrono>
#include "Audio/AudioManager.hpp"
#include "Types/Atlas.hpp"
#include "Nodes/Sprite.hpp"
#include "Nodes/AnimatedSprite.hpp"
#include "Util/Logging.hpp"
#include "MarionetteUI/Label.hpp"
#include "Input/InputManager.hpp"
#include "MarionetteUI/Button.hpp"
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

    FWE::MarionetteUI::Font font = {uiManager->LoadFont("resources/fonts/fnaf_world_font.ttf"), 32};

    FWE::Renderer::Image img = imgLoader->LoadImage("resources/Background.png");

    // FWE::Types::Atlas atlas = FWE::Types::Atlas::CreateFromImage(img);

    FWE::MarionetteUI::TextInput input({0, 0}, {200, 50}, font, img, false, "Temp", FWE::MarionetteUI::HorizontalAlignment::Center, FWE::MarionetteUI::VerticalAlignment::Middle);

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