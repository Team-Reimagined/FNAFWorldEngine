#pragma once

#include <cstdint>
#include <string>

namespace FWE::Renderer
{
    struct Image
    {
        std::string filePath;
        uint32_t width;
        uint32_t height;
        int n;
        int id = -1;
    };
};