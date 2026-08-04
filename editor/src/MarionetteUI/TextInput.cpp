#include "MarionetteUI/TextInput.hpp"

namespace FWE::MarionetteUI
{
    TextInput::TextInput(glm::vec2 position, glm::vec2 size, Font font, Types::Atlas atlas, bool tile, const char *placeholderText, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment) : 
    userLabel(position, "", font, horizontalAlignment, verticalAlignnment), 
    placeholderLabel(position, placeholderText, font, horizontalAlignment, verticalAlignnment), 
    panel(position, size, atlas, tile, horizontalAlignment, verticalAlignnment), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        
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