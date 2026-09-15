#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include "Types/Color.hpp"

namespace FWE::MarionetteUI
{
    struct Font
    {
        TTF_Font *font;
        float fontSize = 12;
        Types::Color color = 0xFFFFFFFF;
    };
}