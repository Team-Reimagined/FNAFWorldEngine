#pragma once

#include "Renderer/Image.hpp"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include <msdf-atlas-gen/GlyphBox.h>

namespace FWE::Types
{
    struct FontAtlas
    {
        Renderer::Image image;
        std::vector<msdf_atlas::GlyphGeometry> layout;
        float scale;
    };
}