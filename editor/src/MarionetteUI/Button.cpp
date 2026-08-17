#include "MarionetteUI/Button.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "Types/Color.hpp"

namespace FWE::MarionetteUI
{
    Button::Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, Types::Atlas atlas, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    label(position, str, font, horizontalAlignment, verticalAlignnment), 
    panel(position, size, atlas, tile, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        label.topLevel = false;
        label.horizontalAlignment = HorizontalAlignment::Center;
        label.verticalAlignnment = VerticalAlignment::Middle;
        label.MakeInternal(this);
    }

    Button::Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, Renderer::Image image, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    label(position, str, font, horizontalAlignment, verticalAlignnment), 
    panel(position, size, image, tile, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        label.topLevel = false;
        label.horizontalAlignment = HorizontalAlignment::Center;
        label.verticalAlignnment = VerticalAlignment::Middle;
        label.MakeInternal(this);
    }

    Button::Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    label(position, str, font, horizontalAlignment, verticalAlignnment), 
    panel(position, size, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        label.topLevel = false;
        label.horizontalAlignment = HorizontalAlignment::Center;
        label.verticalAlignnment = VerticalAlignment::Middle;
        label.MakeInternal(this);
    }


    void Button::SetCallback(std::function<void()> callback)
    {
        this->callback = callback;
    }

    void Button::Draw()
    {
        panel.Draw();
        label.Draw();
    }

    void Button::Pressed()
    {
        callback();
    }
}