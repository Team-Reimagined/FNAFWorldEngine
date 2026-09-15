#include "MarionetteUI/Button.hpp"
#include "MarionetteUI/Label.hpp"
#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "Types/Color.hpp"

namespace FWE::MarionetteUI
{
    Button::Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, Types::Atlas atlas, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    label({0, 0}, str, font, horizontalAlignment, verticalAlignnment), 
    panel({0, 0}, size, atlas, tile, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        label.topLevel = false;
        label.horizontalAlignment = HorizontalAlignment::Center;
        label.verticalAlignnment = VerticalAlignment::Center;
        label.MakeInternal(this);
        panel.topLevel = false;
        panel.MakeInternal(this);
    }

    Button::Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, Renderer::Image image, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    label({0, 0}, str, font, horizontalAlignment, verticalAlignnment), 
    panel({0, 0}, size, image, tile, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        label.topLevel = false;
        label.horizontalAlignment = HorizontalAlignment::Center;
        label.verticalAlignnment = VerticalAlignment::Center;
        label.MakeInternal(this);
        panel.topLevel = false;
        panel.MakeInternal(this);
    }

    Button::Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    label({0, 0}, str, font, horizontalAlignment, verticalAlignnment), 
    panel({0, 0}, size, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        label.topLevel = false;
        label.horizontalAlignment = HorizontalAlignment::Center;
        label.verticalAlignnment = VerticalAlignment::Center;
        label.MakeInternal(this);
        panel.topLevel = false;
        panel.MakeInternal(this);
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

    void Button::OnLeftClick()
    {
        callback();
    }
}