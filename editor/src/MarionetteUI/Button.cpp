#include "MarionetteUI/Button.hpp"
#include "MarionetteUI/UIManager.hpp"

namespace FWE::MarionetteUI
{
    Button::Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, Types::Atlas atlas, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment) : 
    label(position, str, font, horizontalAlignment, verticalAlignnment), 
    panel(position, size, atlas, tile, horizontalAlignment, verticalAlignnment), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        
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