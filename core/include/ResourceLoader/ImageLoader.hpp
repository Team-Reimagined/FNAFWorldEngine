#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>
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
        ~ImageLoader();
        static ImageLoader *GetInstance();
        FWE::Renderer::Image LoadImage(const char *filePath);
    private:
        std::unordered_map<std::string, Renderer::Image> imageData;
    };
}