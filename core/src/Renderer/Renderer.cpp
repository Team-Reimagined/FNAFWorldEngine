#include "Renderer/Renderer.hpp"
#include <SDL3/SDL_vulkan.h>

namespace FWE::Renderer
{
    void Renderer::Init(bool fixedResolution, bool fullscreen)
    {
        vulkan.Init(fixedResolution, fullscreen);
    }

    void Renderer::Shutdown()
    {
        vulkan.Shutdown();
    }

    void Renderer::Draw(const FWE::Types::Atlas &atlas, int x, int y, float scaleX, float scaleY)
    {
        if(atlas.img.data != nullptr)
        {
            vulkan.Draw(atlas, x, y, scaleX, scaleY);
        }
        //SDL_RenderTexture(renderer, image.texture, NULL, &image.position);
    }

    void Renderer::Render()
    {
        vulkan.Render();
        //SDL_RenderPresent(renderer);
        //SDL_RenderClear(renderer);
    }

    Renderer *Renderer::GetInstance()
    {
        static Renderer instance;
        return &instance;
    }

    int Renderer::GetImageId(const Image &image)
    {
        return vulkan.AddImage(image);
    }
};