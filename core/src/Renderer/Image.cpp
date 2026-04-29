#include "Renderer/Image.hpp"
#include "Renderer/Renderer.hpp"

namespace FWE::Renderer
{
    Image::Image(const char *filePath, int x, int y, float scaleX, float scaleY)
    {
        texture = IMG_LoadTexture(Renderer::GetInstance()->GetRenderer(), filePath);
        SDL_GetTextureSize(texture, &position.w, &position.h);
        position.x = x;
        position.y = y;
        position.w *= scaleX;
        position.h *= scaleY;
    }

    Image::~Image()
    {
        SDL_DestroyTexture(texture);
    }
};
