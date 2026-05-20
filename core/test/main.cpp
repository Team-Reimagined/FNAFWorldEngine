#include "Renderer/Renderer.hpp"
#include "Audio/AudioManager.hpp"
#include <thread>
#include <chrono>
#include "Audio/AudioManager.hpp"
#include "backends/imgui_impl_sdl3.h"
#include "Types/Atlas.hpp"
#include "Nodes/Sprite.hpp"
#include "Nodes/AnimatedSprite.hpp"
#include "Util/Logging.hpp"
#include <backends/imgui_impl_vulkan.h>

int main() {
    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();

    const bool fixedResolution = false;
    const bool fullscreen = false;

    renderer->Init(fixedResolution, fullscreen);
  
    FWE::Audio::AudioManager* am = FWE::Audio::AudioManager::GetInstance();

    am->Init();
    auto audio = am->LoadAudio("bulletproof.mp3");
    auto track = am->CreateTrack(audio);
    track->SetVolume(1.75f);
    track->Play();

    FWE::Renderer::Image backgroundImg("Background.png");
    FWE::Types::Atlas backgroundAtlas =
    {
        .img = backgroundImg,
        .x = 0,
        .y = 0,
        .width = 800,
        .height = 480
    };
  
    FWE::Renderer::Image freddyBonnieImg("FreddyBonnieAtlas.png");
    FWE::Types::Atlas freddyBonnieAtlas = 
    {
        .img = freddyBonnieImg,
        .x = 0,
        .y = 0,
        .width = 250,
        .height = 250
    };

    FWE::Nodes::Sprite background;
    background.atlas = backgroundAtlas;
    background.position = {400, 240};
    background.scale = {1, 1};

    FWE::Nodes::AnimatedSprite freddy;
    freddy.atlas = freddyBonnieAtlas;
    freddy.frameCount = 11;
    freddy.position = {300, 240};

    FWE::Nodes::AnimatedSprite bonnie;
    bonnie.atlas = freddyBonnieAtlas;
    bonnie.atlas.y = 250;
    bonnie.frameCount = 11;
    bonnie.position = {500, 240};

    bool running = true;
    while (running)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
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

        background.Draw();
        freddy.Draw();
        bonnie.Draw();
        renderer->Render();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Stats");
		ImGui::Text("Stat");
        ImGui::End();

        ImGui::Render();
    }
    renderer->Shutdown();
}