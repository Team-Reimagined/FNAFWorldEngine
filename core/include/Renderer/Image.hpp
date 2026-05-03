#pragma once

#include <cstdint>

namespace FWE::Renderer
{
    class Image
    {
    public:
        Image(const char *filePath, int x = 0, int y = 0, float scaleX = 1, float scaleY = 1);
        ~Image();
    public:
        void *data;
        int x;
        int y;
        uint32_t width;
        uint32_t height;
        int n;
        float scaleX;
        float scaleY;
        int id;
    };
};