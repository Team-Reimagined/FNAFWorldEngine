#pragma once

#include <SDL3_image/SDL_image.h>

namespace FWE::Renderer
{
    class Image
    {
    public:
        Image(const char *filePath, int x = 0, int y = 0, float scaleX = 1, float scaleY = 1);
        ~Image();
    public:
        SDL_Texture *texture;
        SDL_FRect position;
    };
};