#include "Audio/Track.hpp"
#include "Renderer/Renderer.hpp"
#include "Audio/AudioManager.hpp"
#include "Audio/AudioManager.hpp"
#include "Nodes/Initialize.hpp"
#include "ResourceLoader/FontLoader.hpp"
#include "Scenes/SceneManager.hpp"
#include "Input/InputManager.hpp"
#include "ResourceLoader/AudioLoader.hpp"
#include "Types/FontAtlas.hpp"
#include <stdbool.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

int main() {

    FWE::Nodes::Initalize();

    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();

    const bool fixedResolution = false;
    const bool fullscreen = false;

    renderer->Init(fixedResolution, fullscreen);
  
    FWE::Audio::AudioManager* audioManager = FWE::Audio::AudioManager::GetInstance();
    FWE::ResourceLoader::AudioLoader *audioLoader = FWE::ResourceLoader::AudioLoader::GetInstance();

    audioManager->Init();

    FWE::Audio::Track honk = audioLoader->LoadAudio("resources/PartyFavorraspyPart_AC01__3.wav");
    honk.SetVolume(.25);
    honk.Play();

    FWE::Audio::Track bgm = audioLoader->LoadAudio("resources/Title screen theme - One for the road draft1.wav");
    bgm.SetVolume(.25);
    bgm.Play(true);

    FWE::Scenes::SceneManager *sceneManager = FWE::Scenes::SceneManager::GetInstance();
    sceneManager->LoadScene("resources/TestScene.scene"); 

    FWE::Input::InputManager *inputManager = FWE::Input::InputManager::GetInstance();
    
    bool running = true;

    FWE::Types::FontAtlas *font = FWE::ResourceLoader::FontLoader::GetInstance()->LoadFont("resources/fonts/Roboto-VariableFont_wdth,wght.ttf");

    while (running)
    {
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
        renderer->DrawFont(font, "Hello FNaF World", {400, 75}, 72);
        renderer->DrawFont(font, "Hello FNaF World", {400, 125}, 24);
        renderer->Render();
    }
    renderer->Shutdown();
}