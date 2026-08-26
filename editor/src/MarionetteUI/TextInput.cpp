#include "MarionetteUI/TextInput.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "Types/Color.hpp"

namespace FWE::MarionetteUI
{
    TextInput::TextInput(glm::vec2 position, glm::vec2 size, Font font, Types::Atlas atlas, bool tile, const char *placeholderText, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    userLabel(position, "", font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    placeholderLabel(position, placeholderText, font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    panel(position, size, atlas, tile, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        userLabel.topLevel = false;
        placeholderLabel.topLevel = false;
        userLabel.MakeInternal(this);
        placeholderLabel.MakeInternal(this);
    }

    TextInput::TextInput(glm::vec2 position, glm::vec2 size, Font font, Renderer::Image image, bool tile, const char *placeholderText, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    userLabel(position, "", font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    placeholderLabel(position, placeholderText, font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    panel(position, size, image, tile, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        userLabel.topLevel = false;
        placeholderLabel.topLevel = false;
        userLabel.MakeInternal(this);
        placeholderLabel.MakeInternal(this);
    }

    TextInput::TextInput(glm::vec2 position, glm::vec2 size, Font font, const char *placeholderText, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    userLabel(position, "", font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    placeholderLabel(position, placeholderText, font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    panel(position, size, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        userLabel.topLevel = false;
        placeholderLabel.topLevel = false;
        userLabel.MakeInternal(this);
        placeholderLabel.MakeInternal(this);
    }

    void TextInput::SetPlaceholderText(const char *str)
    {
        placeholderLabel.SetText(str);
    }

    void TextInput::SetFont(TTF_Font *font)
    {
        userLabel.SetFont(font);
        placeholderLabel.SetFont(font);
    }

    void TextInput::SetFontSize(float size)
    {
        userLabel.SetFontSize(size);
        placeholderLabel.SetFontSize(size);
    }

    void TextInput::SetFontColor(Types::Color color)
    {
        userLabel.SetFontColor(color);
    }

    void TextInput::SetPlaceholderFontColor(Types::Color color)
    {
        placeholderLabel.SetFontColor(color);
    }

    void TextInput::Draw()
    {
        panel.Draw();
        if(userInput == "")
        {
            if(placeholderLabel.GetTextSize().x > size.x)
            {
                float scale = size.x / placeholderLabel.GetTextSize().x;
                placeholderLabel.size = {placeholderLabel.GetTextSize().x * scale, placeholderLabel.GetTextSize().y * scale};
            }
            else
            {
                placeholderLabel.size = placeholderLabel.GetTextSize();
            }
            placeholderLabel.Draw();
        }
        else
        {
            if(userLabel.GetTextSize().x > size.x)
            {
                float scale = size.x / userLabel.GetTextSize().x;
                userLabel.size = {userLabel.GetTextSize().x * scale, userLabel.GetTextSize().y * scale};
            }
            else
            {
                userLabel.size = userLabel.GetTextSize();
            }
            userLabel.Draw();
        }
    }

    void TextInput::AddCharacter(const char *character)
    {
        userInput += character;
        userLabel.SetText(userInput.c_str());
    }

    void TextInput::RemoveCharacter()
    {
        if(userInput.length() == 0)
        {
            return;
        }
        userInput.pop_back();
        userLabel.SetText(userInput.c_str());
    }
}