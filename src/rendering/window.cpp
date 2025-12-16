#include "rendering/window.hpp"

#include <spdlog/spdlog.h>

namespace FNAFWorldEngine::Rendering {
    bool Window::createWindow(int width, int height, std::string winTitle) {
        this->winWidth = width;
        this->winHeight = height;
        this->winTitle = winTitle;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
            spdlog::error("SDL_Init Error: {}", SDL_GetError());
            return false;
        }

        this->win = SDL_CreateWindow(winTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
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
    }

    void Window::renderLoop() {
        SDL_Event e;
        while (!this->quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            SDL_RenderClear(this->renderer);
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