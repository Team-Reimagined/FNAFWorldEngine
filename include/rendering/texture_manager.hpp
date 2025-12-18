#pragma once

#include "util/Singleton.hpp"

#include <string>
#include <optional>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace FNAFWorldEngine {
    namespace Rendering {
        class TextureManager : public FNAFWorldEngine::Utils::Singleton<TextureManager> {
        public:
            /**
             * @brief Creates a SDL_Texture based off of a path
             * 
             * @param[in] path The path of the image to render
             * @param[in] ren The SDL_Renderer
             * 
             * @return An SDL_Texture containing the image data
             */
            SDL_Texture* createTexture(std::string path, SDL_Renderer* ren);
            
            /**
             * @brief Renders a SDL_Texture to the screen
             * 
             * @param[in] tex The texture to render
             * @param[in] ren The SDL_Renderer
             * @param[in] x The x coordinate to render at
             * @param[in] y The y coordinate to render at
             */
            void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);

            /**
             * @brief Renders a SDL_Texture to the screen
             * 
             * @param[in] tex The texture to render
             * @param[in] ren The SDL_Renderer
             * @param[in] x The x coordinate to render at
             * @param[in] y The y coordinate to render at
             * @param[in] w The width to render at
             * @param[in] h The height to render at
             */
            void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);
        };
    }
}