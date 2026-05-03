#pragma once

#include <cstdint>

namespace FWE::Renderer
{
    class Image
    {
    public:
        Image(const char *filePath);
        ~Image();
    public:
        void *data;
        uint32_t width;
        uint32_t height;
        int n;
        int id;
    };
};