#pragma once

#include "Label.hpp"
#include <string>
#include "Panel.hpp"
#include "Types/Color.hpp"

namespace FWE::MarionetteUI
{
    class TextInput : public UIElement
    {
    public:
        TextInput(glm::vec2 position, glm::vec2 size, Font font, Types::Atlas atlas, bool tile = false, const char *placeholderText = "", HorizontalAlignment horizontalAlignment = Left, VerticalAlignment verticalAlignnment = Top, Types::Color color = 0xFFFFFFFF);
        TextInput(glm::vec2 position, glm::vec2 size, Font font, Renderer::Image image, bool tile = false, const char *placeholderText = "", HorizontalAlignment horizontalAlignment = Left, VerticalAlignment verticalAlignnment = Top, Types::Color color = 0xFFFFFFFF);
        TextInput(glm::vec2 position, glm::vec2 size, Font font, const char *placeholderText = "", HorizontalAlignment horizontalAlignment = Left, VerticalAlignment verticalAlignnment = Top, Types::Color color = 0xFFFFFFFF);
        void SetPlaceholderText(const char *str);
        void SetFont(TTF_Font *font);
        void SetFontSize(float size);
        void SetFontColor(Types::Color color);
        void SetPlaceholderFontColor(Types::Color color);
        void Draw() override;
        void AddCharacter(const char *character);
        void RemoveCharacter();
    private:
        Label userLabel;
        Label placeholderLabel;
        Panel panel;
        std::string userInput;
    };
}