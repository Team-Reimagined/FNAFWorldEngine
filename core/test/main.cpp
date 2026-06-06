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
#include "Nodes/Initialize.hpp"
#include "ResourceLoader/ImageLoader.hpp"
#include "Scenes/SceneManager.hpp"
#include "UI/UIPanel.hpp"
#include "UI/UIHelper.hpp"
#include "misc/cpp/imgui_stdlib.h"

int main() {

    FWE::Nodes::Initalize();

    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();

    const bool fixedResolution = false;
    const bool fullscreen = false;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    renderer->Init(fixedResolution, fullscreen);
  
    FWE::Audio::AudioManager* am = FWE::Audio::AudioManager::GetInstance();

    am->Init();
    auto audio = am->LoadAudio("bulletproof.mp3");
    auto track = am->CreateTrack(audio);
    track->SetVolume(1.75f);
    track->Play();

    FWE::Scenes::SceneManager *sceneManager = FWE::Scenes::SceneManager::GetInstance();
    sceneManager->LoadScene("resources/TestScene.scene"); 
    
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

        if(!sceneManager->Update())
        {
            running = false;
        }
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