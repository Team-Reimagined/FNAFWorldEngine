#pragma once
#include "Types/FontAtlas.hpp"
#include "Util/Singleton.hpp"
#include <SDL3/SDL.h>
#include "UIElement.hpp"

namespace FWE::MarionetteUI
{
    class UIManager : public FWE::Util::Singleton<UIManager>
    {
    public:
        void Init();
        void Shutdown();
        Types::FontAtlas *LoadFont(const char *filePath);
        Types::FontAtlas *GetDefaultFont();
        void AddUIElementToTree(UIElement *element);
        void Draw();
        void GrabFocus(UIElement *element);
        void ReleaseFocus(UIElement *element);
    private:
        void ProccessInputEvent(const SDL_Event *event);
        void GetElementClicked(glm::vec2 mousePos);
    private:
        bool initalized = false;
        UIElement root;
        UIElement *selected = nullptr;
        Types::FontAtlas *defaultFont;
    };
}