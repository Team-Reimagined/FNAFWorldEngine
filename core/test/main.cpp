#include "Renderer/Renderer.hpp"

int main() {
    FWE::Renderer::Renderer *renderer = FWE::Renderer::Renderer::GetInstance();
    renderer->Init();
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
        }
        renderer->Render();
    }
    renderer->Shutdown();
}