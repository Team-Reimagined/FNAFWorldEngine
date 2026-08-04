#pragma once

#include "Renderer/Image.hpp"

namespace FWE::Types
{
    struct Atlas
    {
        FWE::Renderer::Image img;
        int x = 0;
        int y = 0;
        uint32_t width;
        uint32_t height;
    
        static Atlas CreateFromImage(FWE::Renderer::Image img) {
            return {img, 0, 0, img.width, img.height};
        }   
    };
}