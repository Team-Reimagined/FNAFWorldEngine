#pragma once
#include "Util/Singleton.hpp"
#include <unordered_map>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include "UIElement.hpp"

namespace FWE::MarionetteUI
{
    class UIManager : public FWE::Util::Singleton<UIManager>
    {
    public:
        void Init();
        void Shutdown();
        TTF_Font *LoadFont(const char *filePath);
        TTF_Font *GetDefaultFont();
        void AddUIElementToTree(UIElement *element);
        void Draw();
        void GrabFocus(UIElement *element);
        void ReleaseFocus(UIElement *element);
    private:
        void ProccessInputEvent(const SDL_Event *event);
        void GetElementClicked(glm::vec2 mousePos);
    private:
        bool initalized = false;
        std::unordered_map<std::string, TTF_Font *> fonts;
        UIElement root;
        UIElement *selected = nullptr;
        TTF_Font *defaultFont;
    };
}