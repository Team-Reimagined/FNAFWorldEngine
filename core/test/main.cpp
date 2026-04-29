#include "Renderer/Renderer.hpp"
#include <thread>
#include "Audio/AudioManager.hpp"
#include "backends/imgui_impl_sdl3.h"

int main() {
    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();
    renderer->Init();
  
    FWE::Audio::AudioManager* am = FWE::Audio::AudioManager::GetInstance();

    am->Init();
    auto audio = am->LoadAudio("bulletproof.mp3");
    auto track = am->CreateTrack(audio);
    track->SetVolume(1.75f);
    track->Play();
  
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
            ImGui_ImplSDL3_ProcessEvent(&events);
        }
        renderer->Render();
    }
    renderer->Shutdown();
}