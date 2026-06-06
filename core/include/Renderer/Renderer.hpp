#pragma once

#include <SDL3/SDL.h>
#include "ResourceLoader/ImageLoader.hpp"
#include "Renderer/Vulkan.hpp"
#include "Types/Atlas.hpp"

namespace FWE::Renderer
{
    class Renderer
    {
    public:
        void Init(bool fixedResolution, bool fullscreen);
        void Shutdown();
        void Draw(const FWE::Types::Atlas &atlas, int x = 0, int y = 0, float scaleX = 1, float scaleY = 1, float tileX = 1, float tileY = 1);
        void Render();
        int AddImage(const ResourceLoader::ImageResource &image);
        SDL_Window* GetWindow();
        static Renderer *GetInstance();
    private:
        SDL_Window *window;
        const char *windowTitle = "Fnaf World Engine";
        const int width = 800;
        const int height = 480;

        Vulkan::Vulkan vulkan;
    };
};
