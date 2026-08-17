#pragma once

#include "Types/Atlas.hpp"
#include "Types/Color.hpp"
#include "UIElement.hpp"

namespace FWE::MarionetteUI
{
    class Panel : public UIElement
    {
    public:
        Panel(glm::vec2 position, glm::vec2 size, Types::Atlas atlas, bool tile = false, HorizontalAlignment horizontalAlignment = Left, VerticalAlignment verticalAlignnment = Top, Types::Color color = 0xFFFFFFFF);
        Panel(glm::vec2 position, glm::vec2 size, Renderer::Image image, bool tile = false, HorizontalAlignment horizontalAlignment = Left, VerticalAlignment verticalAlignnment = Top, Types::Color color = 0xFFFFFFFF);
        Panel(glm::vec2 position, glm::vec2 size, HorizontalAlignment horizontalAlignment = Left, VerticalAlignment verticalAlignnment = Top, Types::Color color = 0xFFFFFFFF);
        void Draw() override;
    public:
        bool tile = false;
        Types::Color color;
    private:
        Types::Atlas atlas;
    };
}