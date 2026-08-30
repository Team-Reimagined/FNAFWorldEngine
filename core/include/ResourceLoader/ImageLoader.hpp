#pragma once

#include "Renderer/Image.hpp"


namespace FWE::ResourceLoader
{
    struct ImageResource
    {
        void *data;
        Renderer::Image image;
    };
    
    class ImageLoader
    {
    public:
        static ImageLoader *GetInstance();
        FWE::Renderer::Image LoadImage(const char *filePath);
        void Clear();
    private:
        std::unordered_map<std::string, Renderer::Image> imageData;
    };
}