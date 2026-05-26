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

    void Renderer::Draw(const FWE::Types::Atlas &atlas, int x, int y, float scaleX, float scaleY, float tileX, float tileY)
    {
        if(atlas.img.id != -1)
        {
            vulkan.Draw(atlas, x, y, scaleX, scaleY, tileX, tileY);
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

    int Renderer::AddImage(const ResourceLoader::ImageResource &image)
    {
        return vulkan.AddImage(image);
    }
};