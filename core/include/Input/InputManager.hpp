#pragma once
#include <SDL3/SDL.h>
#include <functional>
#include <vector>

namespace FWE::Input
{
    class InputManager
    {
    public:
        static InputManager *GetInstance();
        void ClearBuffers();
        void ProcessEvent(const SDL_Event *event);
        bool IsKeyPressed(SDL_Keycode key);
        bool IsKeyJustPressed(SDL_Keycode key);
        bool IsKeyJustReleased(SDL_Keycode key);
        bool IsPhysicalKeyPressed(SDL_Scancode key);
        bool IsPhysicalKeyJustPressed(SDL_Scancode key);
        bool IsPhysicalKeyJustReleased(SDL_Scancode key);
        void AddInputCallback(std::function<void(const SDL_Event *event)> callback);
    private:
        bool keysPressed[SDL_SCANCODE_COUNT];
        bool keysJustPressed[SDL_SCANCODE_COUNT];
        bool keysJustReleased[SDL_SCANCODE_COUNT];
        std::vector<std::function<void(const SDL_Event *event)>> inputCallbacks;
    };
}