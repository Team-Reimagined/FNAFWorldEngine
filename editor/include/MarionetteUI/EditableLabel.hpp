#pragma once

#include "Label.hpp"
#include <string>
#include "Panel.hpp"
#include "Types/Color.hpp"
#include "Types/FontAtlas.hpp"

namespace FWE::MarionetteUI
{
    class EditableLabel : public UIElement
    {
    public:
        EditableLabel(glm::vec2 position, glm::vec2 size, Font font, Types::Atlas atlas, bool tile = false, const char *placeholderText = "", HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignnment = VerticalAlignment::Top, Types::Color color = 0xFFFFFFFF);
        EditableLabel(glm::vec2 position, glm::vec2 size, Font font, Renderer::Image image, bool tile = false, const char *placeholderText = "", HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignnment = VerticalAlignment::Top, Types::Color color = 0xFFFFFFFF);
        EditableLabel(glm::vec2 position, glm::vec2 size, Font font, const char *placeholderText = "", HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignnment = VerticalAlignment::Top, Types::Color color = 0xFFFFFFFF);
        void SetPlaceholderText(const char *str);
        void SetFont(Types::FontAtlas *font);
        void SetFontSize(float size);
        void SetFontColor(Types::Color color);
        void SetPlaceholderFontColor(Types::Color color);
        void Draw() override;
        void KeyPressed(SDL_Keycode key) override;
        void TextInput(const char *text) override;
    private:
        void RemoveCharacter();
    private:
        Label userLabel;
        Label placeholderLabel;
        Panel panel;
        std::string userInput;
    };
}