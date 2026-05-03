#include "Renderer/Image.hpp"
#include "Renderer/Renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"



namespace FWE::Renderer
{
    Image::Image(const char *filePath)
    {
        data = stbi_load(filePath, (int *)&width, (int *)&height, &n, 4);

        if(data == nullptr)
        {
            Util::Logging::error("Unable to load image at path: {}", filePath);
            return;
        }
        
        id = Renderer::GetInstance()->GetImageId(*this);
    }

    Image::~Image()
    {
        stbi_image_free(data);
    }
};
