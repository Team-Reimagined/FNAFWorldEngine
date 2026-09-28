#include "ResourceLoader/FontLoader.hpp"
#include "Renderer/Image.hpp"
#include "Renderer/Renderer.hpp"
#include "ResourceLoader/ImageLoader.hpp"
#include "Types/FontAtlas.hpp"
#include "Util/Logging.hpp"
#include <cstdint>
#include <vector>

namespace FWE::ResourceLoader
{
    Types::FontAtlas *FontLoader::LoadFont(const char *filePath)
    {
        using namespace msdf_atlas;
        if(fontData.find(filePath) != fontData.end())
        {
            return &fontData.at(filePath);
        }
        else
        {
            if(freeTypeHandle == nullptr)
            {
                freeTypeHandle = msdfgen::initializeFreetype();
            }
            msdfgen::FontHandle *fontHandle = msdfgen::loadFont(freeTypeHandle, filePath);
            if(fontHandle == nullptr)
            {
                Util::Logging::error("Cannot load font at path {}", filePath);
                return nullptr;
            }
            fontData.insert({filePath, {}});
            Types::FontAtlas &fontAtlas = fontData.at(filePath);
            std::vector<GlyphGeometry> &glyphs = fontAtlas.layout;
            FontGeometry fontGeomentry(&glyphs);
            fontGeomentry.loadCharset(fontHandle, 1, Charset::ASCII);

            for(GlyphGeometry &glyph : glyphs)
            {
                glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, 3, 0);
            }

            TightAtlasPacker packer;
            packer.setDimensionsConstraint(DimensionsConstraint::SQUARE);
            packer.setMinimumScale(24);
            packer.setPixelRange(2);
            packer.setMiterLimit(1);
            packer.pack(glyphs.data(), glyphs.size());
            fontAtlas.scale = packer.getScale();

            int width;
            int height;

            packer.getDimensions(width, height);

            ImmediateAtlasGenerator<float, 4, mtsdfGenerator, BitmapAtlasStorage<byte, 4>> generator(width, height);

            GeneratorAttributes attributes;
            generator.setAttributes(attributes);
            generator.setThreadCount(4);
            generator.generate(glyphs.data(), glyphs.size());

            msdfgen::BitmapConstRef<byte, 4> rgba = generator.atlasStorage();
            
            ImageResource imgResource = {(void *)rgba.pixels, {(uint32_t)width, (uint32_t)height, 4}};
            Renderer::Image &img = fontAtlas.image;
            
            imgResource.image.allocatedImg = FWE::Renderer::Renderer::GetInstance()->AddImage(imgResource);

            img = imgResource.image;

            msdfgen::destroyFont(fontHandle);
            
            return &fontAtlas; 
        }
    }

    FontLoader *FontLoader::GetInstance()
    {
        static FontLoader loader;
        return &loader;
    }
}