#include "Renderer/Image.hpp"
#include "Renderer/Renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"



namespace FWE::Renderer
{
    Image::Image(const char *filePath, int x, int y, float scaleX, float scaleY)
    {
        data = stbi_load(filePath, (int *)&width, (int *)&height, &n, 4);

        if(data == nullptr)
        {
            Util::Logging::error("Unable to load image at path: {}", filePath);
            return;
        }
        
        this->x = x;
        this->y = y;
        this->scaleX = scaleX;
        this->scaleY = scaleY;
        id = Renderer::GetInstance()->GetImageId(*this);
    }

    Image::~Image()
    {
        stbi_image_free(data);
    }
};
