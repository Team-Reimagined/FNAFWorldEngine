#include "Renderer/Renderer.hpp"
#include <SDL3/SDL_vulkan.h>

namespace FWE::Renderer
{
    void Renderer::Init()
    {
        vulkan.Init();
    }

    void Renderer::Shutdown()
    {
        vulkan.Shutdown();
    }

    void Renderer::Draw(const Image &image, int x, int y, float scaleX, float scaleY)
    {
        if(image.data != nullptr)
        {
            vulkan.Draw(image);
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