#pragma once

#include <string>

#include <SDL2/SDL.h>

#define ENGINE_VERSION "0.0.1"

namespace FNAFWorldEngine {
    namespace Rendering {
        class Window {
        public:
            /**
             * @brief Creates a window
             * 
             * @param[in] width The width of the window
             * @param[in] height The height of the window
             * @param[in] winTitle The title of the window
             * 
             * @return A bool stating if the window was created or not
             */
            bool createWindow(int width, int height, std::string winTitle);

            /**
             * @brief The render loop for the window
             */
            void renderLoop();

            /**
             * @brief Cleanup SDL2 on quit
             */
            void cleanup();

        private:
            int winWidth, winHeight;
            std::string winTitle;
            bool quit;

            SDL_Window* win;
            SDL_Renderer* renderer;

        };
    }
}