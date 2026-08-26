#pragma once

#include <string>
#include "Types/Atlas.hpp"
#include "UIElement.hpp"
#include "Font.hpp"

namespace FWE::MarionetteUI
{
    class Label : public UIElement
    {
    public:
        Label(glm::vec2 position, const char *str, Font font, HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignnment = VerticalAlignment::Top);
        void SetText(const char *str);
        void SetFont(TTF_Font *font);
        void SetFontSize(float size);
        void SetFontColor(Types::Color color);
        void Draw() override;
        glm::vec2 GetTextSize();
    private:
        void Generate();
    private:
        std::string text;
        Font font;
        Types::Atlas texture;
    };
}