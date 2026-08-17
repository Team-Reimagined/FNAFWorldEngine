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

    void Renderer::Draw(const FWE::Types::Atlas &atlas, float x, float y, float scaleX, float scaleY, float tileX, float tileY, Types::Color color)
    {
        if(atlas.img.id != -1)
        {
            vulkan.Draw(atlas, x, y, scaleX, scaleY, tileX, tileY, color);
        }
        //SDL_RenderTexture(renderer, image.texture, NULL, &image.position);
    }

    void Renderer::Draw(float x, float y, float sizeX, float sizeY, Types::Color color)
    {
        vulkan.Draw(x, y, sizeX, sizeY, color);
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

    void Renderer::RemoveImage(const Image &image)
    {
        vulkan.RemoveImage(image);
    }

    SDL_Window *Renderer::GetWindow()
    {
        return vulkan.GetWindow();
    }
    
};