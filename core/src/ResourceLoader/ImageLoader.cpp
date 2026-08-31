#include "ResourceLoader/ImageLoader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"
#include "Util/Logging.hpp"
#include "Renderer/Renderer.hpp"

namespace FWE::ResourceLoader
{
    Renderer::Image ImageLoader::LoadImage(const char *filePath)
    {
        if(imageData.find(filePath) != imageData.end())
        {
            return imageData.at(filePath);
        }
        else
        {
            ImageResource resource;
            resource.data = stbi_load(filePath, (int *)&resource.image.width, (int *)&resource.image.height, &resource.image.depth, 4);
            if(resource.data != nullptr)
            {
                Renderer::Renderer *renderer = Renderer::Renderer::GetInstance();
                resource.image.allocatedImg = renderer->AddImage(resource);
                stbi_image_free(resource.data);
                imageData.insert({filePath, resource.image});
            }
            else
            {
                Util::Logging::error("Unable to load image at path: {}", filePath);
            }
            return resource.image;
        }
    }

    ImageLoader *ImageLoader::GetInstance()
    {
        static ImageLoader loader;
        return &loader;
    }

    void ImageLoader::Clear()
    {
        for(auto [str, var] : imageData)
        {
            Renderer::Renderer::GetInstance()->RemoveImage(var);
        }
        imageData.clear();
    }
}