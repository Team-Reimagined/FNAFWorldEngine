#pragma once

#include "Types/FontAtlas.hpp"
#include <unordered_map>
#include <string>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

namespace FWE::ResourceLoader
{
    class FontLoader
    {
    public:
        static FontLoader *GetInstance();
        Types::FontAtlas *LoadFont(const char *filePath);
    private:
        std::unordered_map<std::string, Types::FontAtlas> fontData;
        msdfgen::FreetypeHandle *freeTypeHandle = nullptr;
    };
}