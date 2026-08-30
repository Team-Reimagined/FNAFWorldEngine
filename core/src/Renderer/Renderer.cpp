#include "Renderer/Renderer.hpp"
#include "ResourceLoader/ImageLoader.hpp"
#include "glm/ext/vector_float2.hpp"
#include <SDL3/SDL_vulkan.h>

namespace FWE::Renderer
{
    void Renderer::Init(bool fixedResolution, bool fullscreen)
    {
        vulkan.Init(fixedResolution, fullscreen);
    }

    void Renderer::Shutdown()
    {
        ResourceLoader::ImageLoader::GetInstance()->Clear();
        vulkan.Shutdown();
    }

    void Renderer::Draw(const FWE::Types::Atlas &atlas, glm::vec2 position, glm::vec2 scale, glm::vec2 tileCount, Types::Color color)
    {
        if(atlas.img.allocatedImg.image != nullptr)
        {
            vulkan.Draw(atlas, position, scale, tileCount, color);
        }
        //SDL_RenderTexture(renderer, image.texture, NULL, &image.position);
    }

    void Renderer::Draw(glm::vec2 position, glm::vec2 size, Types::Color color)
    {
        vulkan.Draw(position, size, color);
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

    AllocatedImage Renderer::AddImage(const ResourceLoader::ImageResource &image)
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