#pragma once

#include "Label.hpp"
#include "Panel.hpp"
#include <functional>
#include "Types/Color.hpp"
#include "UIElement.hpp"

namespace FWE::MarionetteUI
{
    class Button : public UIElement
    {
    public:
        Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, Types::Atlas atlas, bool tile = false, HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignnment = VerticalAlignment::Top, Types::Color color = 0xFFFFFFFF);
        Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, Renderer::Image image, bool tile = false, HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignnment = VerticalAlignment::Top, Types::Color color = 0xFFFFFFFF);
        Button(glm::vec2 position, glm::vec2 size, const char *str, Font font, HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignnment = VerticalAlignment::Top, Types::Color color = 0xFFFFFFFF);
        void SetCallback(std::function<void()> callback);
        void Pressed();
        void Draw() override;
    public:
        Label label;
        Panel panel;
    private:
        std::function<void()> callback = [](){};
    };
};