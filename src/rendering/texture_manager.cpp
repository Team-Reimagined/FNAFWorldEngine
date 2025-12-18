#include "rendering/texture_manager.hpp"
#include "util/Screen.hpp"

#include <spdlog/spdlog.h>

namespace FNAFWorldEngine::Rendering {
    SDL_Texture* TextureManager::createTexture(std::string path, SDL_Renderer* ren) {
        SDL_Texture* texture = IMG_LoadTexture(ren, path.c_str());
        if (texture == nullptr) {
            spdlog::error("LoadTexture Error: {} | Texture Path: {}", SDL_GetError(), path);
        }

        return texture;
    }

    void TextureManager::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y) {
        int w, h;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        this->renderTexture(tex, ren, x, y, w, h);
    }

    void TextureManager::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h) {
        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(ren, tex, NULL, &dst);
    }
}