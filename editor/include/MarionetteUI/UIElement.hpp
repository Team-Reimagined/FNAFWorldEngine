#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace FWE::MarionetteUI
{
    enum HorizontalAlignment
    {
        Left,
        Center,
        Right,
        FullHorizontal
    };

    enum VerticalAlignment
    {
        Top,
        Middle,
        Bottom,
        FullVertical
    };

    class UIElement
    {
    public:
        UIElement() {};
        UIElement(glm::vec2 position, glm::vec2 size, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment);
        ~UIElement();
        virtual void Draw(){}
        void AddChild(UIElement *element);
        UIElement *GetParent();
        UIElement *GetChild(unsigned int index);
        std::vector<UIElement *> &GetChildren();
        unsigned int GetChildrenCount();
        void RemoveChild(unsigned int index);
        void RemoveFromTree();
        glm::vec2 GetAlignmentOffset();
        void MakeInternal(UIElement *parent);
    public:
        glm::vec2 position = {0, 0};
        glm::vec2 size = {1, 1};
        bool visible = true;
        bool blockMouse = true;
        HorizontalAlignment horizontalAlignment = Left;
        VerticalAlignment verticalAlignnment = Top;
        bool topLevel = true;
    private:
        UIElement *parent = nullptr;
        std::vector<UIElement *> children;
        bool internal = false;
    };
}