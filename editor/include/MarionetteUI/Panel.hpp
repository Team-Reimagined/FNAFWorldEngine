#pragma once

#include <string>
#include "UIManager.hpp"
#include "Renderer/Renderer.hpp"
#include "UIElement.hpp"

namespace FWE::MarionetteUI
{
    class Panel : public UIElement
    {
    public:
        Panel(glm::vec2 position, glm::vec2 size, Types::Atlas atlas, bool tile = false, HorizontalAlignment horizontalAlignment = Left, VerticalAlignment verticalAlignnment = Top);
        void Draw() override;
    public:
        bool tile = false;
    private:
        Types::Atlas atlas;
    };
}