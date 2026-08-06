#include "Input/InputManager.hpp"

namespace FWE::Input
{
    InputManager *InputManager::GetInstance()
    {
        static InputManager manager;
        return &manager;
    }

    void InputManager::ClearBuffers()
    {
        memset(keysJustPressed, 0, sizeof(keysJustPressed));
        memset(keysJustReleased, 0, sizeof(keysJustReleased));
    }
    
    void InputManager::ProcessEvent(const SDL_Event *event)
    {
        switch (event->type)
        {
        case SDL_EVENT_KEY_DOWN:
            if(!event->key.repeat)
            {
                keysJustPressed[event->key.scancode] = true;
                keysPressed[event->key.scancode] = true;
            }
            break;
        case SDL_EVENT_KEY_UP:
            keysJustReleased[event->key.scancode] = true;
            keysPressed[event->key.scancode] = false;
        default:
            break;
        }
        for(auto i : inputCallbacks)
        {
            i(event);
        }
    }

    bool InputManager::IsKeyPressed(SDL_Keycode key)
    {
        return keysPressed[SDL_GetScancodeFromKey(key, nullptr)];
    }
    bool InputManager::IsKeyJustPressed(SDL_Keycode key)
    {
        return keysJustPressed[SDL_GetScancodeFromKey(key, nullptr)];
    }
    bool InputManager::IsKeyJustReleased(SDL_Keycode key)
    {
        return keysJustReleased[SDL_GetScancodeFromKey(key, nullptr)];
    }
    bool InputManager::IsPhysicalKeyPressed(SDL_Scancode key)
    {
        return keysPressed[key];
    }
    bool InputManager::IsPhysicalKeyJustPressed(SDL_Scancode key)
    {
        return keysJustPressed[key];
    }
    bool InputManager::IsPhysicalKeyJustReleased(SDL_Scancode key)
    {
        return keysJustReleased[key];
    }

    void InputManager::AddInputCallback(std::function<void(const SDL_Event *event)> callback)
    {
        inputCallbacks.push_back(callback);
    }
}