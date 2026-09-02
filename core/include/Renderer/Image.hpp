#pragma once

#include "Renderer/AllocatedImage.hpp"
#include <cstdint>

namespace FWE::Renderer
{
    struct Image
    {
        uint32_t width;
        uint32_t height;
        int channels;
        AllocatedImage allocatedImg;
    };
};