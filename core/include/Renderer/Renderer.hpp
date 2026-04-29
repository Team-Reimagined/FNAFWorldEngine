#pragma once

#include <SDL3/SDL.h>
#include "Renderer/Image.hpp"
#include "Renderer/Vulkan.hpp"

namespace FWE::Renderer
{
    class Renderer
    {
    public:
        void Init();
        void Shutdown();
        void Draw(Image image, int x = 0, int y = 0, float scaleX = 1, float scaleY = 1);
        void Render();
        SDL_Renderer *GetRenderer();
        static Renderer *GetInstance();
    private:
        SDL_Renderer *renderer;
        SDL_Window *window;
        const char *windowTitle = "Fnaf World Engine";
        const int width = 800;
        const int height = 480;

        Vulkan vulkan;
    };
};
