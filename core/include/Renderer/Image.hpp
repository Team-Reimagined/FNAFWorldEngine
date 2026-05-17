#pragma once

#include <cstdint>

namespace FWE::Renderer
{
    class Image
    {
    public:
        Image() = default;
        Image(const char *filePath);
        Image(const Image &img) = default;
        ~Image();
    public:
        void *data = nullptr;
        uint32_t width;
        uint32_t height;
        int n;
        int id;
    };
};