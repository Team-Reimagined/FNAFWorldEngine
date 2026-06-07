#include "Input/InputManager.hpp"
#include "Util/Logging.hpp"

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
                Util::Logging::info("Down: {}", event->key.key);
                keysJustPressed[event->key.scancode] = true;
            }
            break;
        case SDL_EVENT_KEY_UP:
            Util::Logging::info("Up: {}", event->key.key);
            keysJustReleased[event->key.scancode] = true;
        default:
            break;
        }
    }
}