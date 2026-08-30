#pragma once

#include <SDL3/SDL.h>
#include "Renderer/AllocatedImage.hpp"
#include "ResourceLoader/ImageLoader.hpp"
#include "Renderer/Vulkan.hpp"
#include "Types/Atlas.hpp"
#include "Types/Color.hpp"
#include "glm/ext/vector_float2.hpp"

namespace FWE::Renderer
{
    class Renderer
    {
    public:
        void Init(bool fixedResolution, bool fullscreen);
        void Shutdown();
        void Draw(const FWE::Types::Atlas &atlas, glm::vec2 position, glm::vec2 scale, glm::vec2 tileCount = {1, 1}, Types::Color color = 0xFFFFFFFF);
        void Draw(glm::vec2 position, glm::vec2 size, Types::Color color = 0xFFFFFFFF);
        void Render();
        AllocatedImage AddImage(const ResourceLoader::ImageResource &image);
        void RemoveImage(const Image &image);
        SDL_Window *GetWindow();
        static Renderer *GetInstance();
    private:
        SDL_Window *window;
        const char *windowTitle = "Fnaf World Engine";
        const int width = 800;
        const int height = 480;

        Vulkan::Vulkan vulkan;
    };
};
