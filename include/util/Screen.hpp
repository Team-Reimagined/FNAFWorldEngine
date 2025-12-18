#pragma once

#include <SDL2/SDL.h>

#include "Singleton.hpp"

namespace FNAFWorldEngine {
    namespace Utils {
        class Screen : public Singleton<Screen> {
        public:
            void initialize(SDL_Window* window) {
                this->window = window;
            }

            int getScreenWidth() {
                int w;
                SDL_GetWindowSize(this->window, &w, NULL);
                return w;
            }

            int getScreenHeight() {
                int h;
                SDL_GetWindowSize(this->window, &h, NULL);
                return h;
            }
        private:
            SDL_Window* window;
        };
    }
}