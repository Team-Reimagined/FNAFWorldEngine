#pragma once
#include <SDL3/SDL.h>

namespace FWE::Input
{
    class InputManager
    {
    public:
        static InputManager *GetInstance();
        void ClearBuffers();
        void ProcessEvent(const SDL_Event *event);
    private:
        bool keysPressed[SDL_SCANCODE_COUNT];
        bool keysJustPressed[SDL_SCANCODE_COUNT];
        bool keysJustReleased[SDL_SCANCODE_COUNT];
    };
}