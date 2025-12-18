#include "rendering/window.hpp"
#include "rendering/texture_manager.hpp"

#include <format>

#include <spdlog/spdlog.h>
#include <SDL2/SDL_image.h>

namespace FNAFWorldEngine::Rendering {
    bool Window::createWindow(int width, int height, std::string winTitle) {
        this->winWidth = width;
        this->winHeight = height;
        this->winTitle = winTitle;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
            spdlog::error("SDL_Init Error: {}", SDL_GetError());
            return false;
        }

        if (!this->initializeImageExtension()) {
            spdlog::error("Initialize SDL_Image error");
            return false;
        }

        this->win = SDL_CreateWindow(
            std::format("{} - Version {}", winTitle, ENGINE_VERSION).c_str(), 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            width, 
            height, 
            SDL_WINDOW_SHOWN
        );
        if (this->win == nullptr) {
            spdlog::error("SDL_CreateWindow Error: {}", SDL_GetError());
            SDL_Quit();
            return false;
        }

        this->renderer = SDL_CreateRenderer(this->win, -1, SDL_RENDERER_ACCELERATED);
        if (this->renderer == nullptr) {
            SDL_DestroyWindow(this->win);
            spdlog::error("SDL_CreateRenderer Error: {}", SDL_GetError());
            SDL_Quit();
            return false;
        }

        this->renderLoop();
        return true;
    }

    bool Window::initializeImageExtension() {
        int flags = IMG_INIT_PNG;
        int imgInit = IMG_Init(flags);

        if ((imgInit & flags) != flags) {
            spdlog::error("IMG_Init Error: {}", IMG_GetError()); //
            SDL_Quit();
            return false;
        }

        return true;
    }

    void Window::renderLoop() {
        SDL_Event e;

        auto testTex = TextureManager::get()->createTexture("resources/test.png", this->renderer);

        while (!this->quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    spdlog::info("hello quit");
                    quit = true;
                }
            }

            SDL_RenderClear(this->renderer);
            TextureManager::get()->renderTexture(testTex, this->renderer, 0, 0, 1280, 720);
            SDL_RenderPresent(this->renderer);
        }

        this->cleanup();
    }

    void Window::cleanup() {
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->win);
        SDL_Quit();
    }
}