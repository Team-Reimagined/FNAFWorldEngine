#include "Audio/Track.hpp"
#include "Renderer/Renderer.hpp"
#include "Audio/AudioManager.hpp"
#include "Audio/AudioManager.hpp"
#include "Nodes/Initialize.hpp"
#include "Scenes/SceneManager.hpp"
#include "Input/InputManager.hpp"
#include "ResourceLoader/AudioLoader.hpp"
#include <stdbool.h>

int main() {

    FWE::Nodes::Initalize();

    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();

    const bool fixedResolution = false;
    const bool fullscreen = false;

    renderer->Init(fixedResolution, fullscreen);
  
    FWE::Audio::AudioManager* audioManager = FWE::Audio::AudioManager::GetInstance();
    FWE::ResourceLoader::AudioLoader *audioLoader = FWE::ResourceLoader::AudioLoader::GetInstance();

    audioManager->Init();

    FWE::Audio::Track track = audioLoader->LoadAudio("resources/PartyFavorraspyPart_AC01__3.wav");
    track.SetVolume(.25);
    track.Play();

    FWE::Scenes::SceneManager *sceneManager = FWE::Scenes::SceneManager::GetInstance();
    sceneManager->LoadScene("resources/TestScene.scene"); 

    FWE::Input::InputManager *inputManager = FWE::Input::InputManager::GetInstance();
    
    bool running = true;

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
        renderer->Render();
    }
    renderer->Shutdown();
}