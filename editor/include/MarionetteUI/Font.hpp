#pragma once

#include "Types/Color.hpp"
#include "Types/FontAtlas.hpp"

namespace FWE::MarionetteUI
{
    struct Font
    {
        Types::FontAtlas *fontAtlas;
        float fontSize = 12;
        Types::Color color = 0xFFFFFFFF;
    };
}