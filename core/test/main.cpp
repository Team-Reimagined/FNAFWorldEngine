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

        FWE::Nodes::Node *node = sceneManager->GetInstance()->GetCurrentScene()->GetRoot()->GetChild(1);
        std::string &nodeName = node->name;
        FWE::UI::UIPanel panel(nodeName, {128*3, 256}, {128, 128}, [=](){
            for(auto [str, var] : node->registeredVariables)
            {
                switch (var.type)
                {
                case FWE::Nodes::Types::Int:
                {
                    ImGui::Text("");
                    ImGui::InputInt(str.c_str(), (int*)var.variable);
                    break;
                }
                case FWE::Nodes::Types::Float:
                {
                    ImGui::Text("");
                    ImGui::InputFloat(str.c_str(), (float*)var.variable);
                    break;
                }
                case FWE::Nodes::Types::Bool:
                {
                    ImGui::Text("");
                    ImGui::Checkbox(str.c_str(), (bool*)var.variable);
                    break;
                }
                case FWE::Nodes::Types::String:
                {
                    ImGui::Text("");
                    ImGui::InputText(str.c_str(), (std::string*)var.variable);
                    break;
                }
                case FWE::Nodes::Types::Vector2:
                {
                    ImGui::Text("");
                    ImGui::Text(str.c_str());
                    ImGui::InputFloat(("X##" + str).c_str(), (float*)&((glm::vec2*)var.variable)->x);
                    ImGui::InputFloat(("Y##" + str).c_str(), (float*)&((glm::vec2*)var.variable)->y);
                    break;
                }
                case FWE::Nodes::Types::Atlas:
                {
                    ImGui::Text("");
                    ImGui::Text(str.c_str());
                    ImGui::Text(("File Path: " + ((FWE::Types::Atlas*)var.variable)->img.filePath).c_str());
                    ImGui::InputInt(("X##" + str).c_str(), (int*)&((FWE::Types::Atlas*)var.variable)->x);
                    ImGui::InputInt(("Y##" + str).c_str(), (int*)&((FWE::Types::Atlas*)var.variable)->y);
                    ImGui::InputFloat(("Width##" + str).c_str(), (float*)&((FWE::Types::Atlas*)var.variable)->width);
                    ImGui::InputFloat(("Height##" + str).c_str(), (float*)&((FWE::Types::Atlas*)var.variable)->height);
                    break;
                }
                }
            }
        });

        ImGui::Render();
    }
    renderer->Shutdown();
}