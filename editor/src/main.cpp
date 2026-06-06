#include "Renderer/Renderer.hpp"
#include "Audio/AudioManager.hpp"
#include <thread>
#include <chrono>
#include "Audio/AudioManager.hpp"
#include "backends/imgui_impl_sdl3.h"
#include "Types/Atlas.hpp"
#include "Nodes/Sprite.hpp"
#include "Nodes/AnimatedSprite.hpp"
#include "UI/FontManager.hpp"
#include "UI/UIHelper.hpp"
#include "Util/Logging.hpp"
#include <backends/imgui_impl_vulkan.h>

#include "Panels/ProjectCreationPanel.hpp"

int main() {
    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();

    const bool fixedResolution = false;
    const bool fullscreen = false;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    FWE::UI::FontManager::Get().Init();

    renderer->Init(fixedResolution, fullscreen);
    
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

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        int w = 0, h = 0;
        SDL_GetWindowSizeInPixels(renderer->GetWindow(), &w, &h);
        ImVec2 windowSize(w, h);

        FWE::UI::UIHelper::CreatePanel("MainPanel", windowSize, ImVec2(0, 0), [=]() {
            auto projectCreationPanel = FWE::Panels::ProjectCreationPanel(FWE::UI::UIHelper::GetCenter(windowSize, FWE::Panels::ProjectCreationPanel::panelSize));
        }, FWE::UI::UIHelper::DEFAULT_WINDOW_FLAGS | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::Render();
    }
    renderer->Shutdown();
}