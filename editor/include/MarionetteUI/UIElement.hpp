#pragma once

#include "SDL3/SDL_keycode.h"
#include <glm/glm.hpp>
#include <vector>

namespace FWE::MarionetteUI
{
    enum class HorizontalAlignment
    {
        Left,
        Center,
        Right,
        Full
    };

    enum class VerticalAlignment
    {
        Top,
        Center,
        Bottom,
        Full
    };

    class UIElement
    {
    public:
        UIElement() {};
        UIElement(glm::vec2 position, glm::vec2 size, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment);
        virtual ~UIElement();

        UIElement(const UIElement &other) = default;
        UIElement(UIElement &&other) noexcept;
        UIElement& operator=(const UIElement& other) = default;
        
        virtual void Draw(){}

        virtual void OnLeftClick(){}
        virtual void OnRightClick(){}
        virtual void OnDrag(glm::vec2 movement){}
        virtual void OnLeftRelease(){}
        virtual void OnRightRelease(){}

        virtual void KeyPressed(SDL_Keycode key){}
        virtual void TextInput(const char *text){}

        virtual void OnFocusGrabbed(){}
        virtual void OnFocusLost(){}

        void GrabFocus();
        void ReleaseFocus();

        void AddChild(UIElement *element);
        UIElement *GetParent();
        UIElement *GetChild(unsigned int index);
        std::vector<UIElement *> &GetChildren();
        unsigned int GetChildrenCount();
        void RemoveChild(unsigned int index);
        void RemoveFromTree();

        glm::vec2 GetGlobalPosition();
        glm::vec2 GetAlignmentOffset();

        void MakeInternal(UIElement *parent);
        
    public:
        glm::vec2 position = {0, 0};
        glm::vec2 size = {1, 1};
        bool visible = true;
        bool blockMouse = true;
        HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left;
        VerticalAlignment verticalAlignnment = VerticalAlignment::Top;
        bool topLevel = true;
    private:
        void AddInternalChild(UIElement *element);
    private:
        UIElement *parent = nullptr;
        std::vector<UIElement *> children;
        std::vector<UIElement *> internalChildren;
        bool internal = false;
    };
}