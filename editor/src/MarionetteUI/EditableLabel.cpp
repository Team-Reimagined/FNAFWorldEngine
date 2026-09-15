#include "MarionetteUI/EditableLabel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "SDL3/SDL_keycode.h"
#include "Types/Color.hpp"

namespace FWE::MarionetteUI
{
    EditableLabel::EditableLabel(glm::vec2 position, glm::vec2 size, Font font, Types::Atlas atlas, bool tile, const char *placeholderText, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    userLabel({0, 0}, "", font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    placeholderLabel({0, 0}, placeholderText, font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    panel({0, 0}, size, atlas, tile, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        userLabel.topLevel = false;
        placeholderLabel.topLevel = false;
        panel.topLevel = false;
        userLabel.MakeInternal(this);
        placeholderLabel.MakeInternal(this);
        panel.MakeInternal(this);
    }

    EditableLabel::EditableLabel(glm::vec2 position, glm::vec2 size, Font font, Renderer::Image image, bool tile, const char *placeholderText, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    userLabel({0, 0}, "", font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    placeholderLabel({0, 0}, placeholderText, font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    panel({0, 0}, size, image, tile, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        userLabel.topLevel = false;
        placeholderLabel.topLevel = false;
        panel.topLevel = false;
        userLabel.MakeInternal(this);
        placeholderLabel.MakeInternal(this);
        panel.MakeInternal(this);
    }

    EditableLabel::EditableLabel(glm::vec2 position, glm::vec2 size, Font font, const char *placeholderText, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : 
    userLabel({0, 0}, "", font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    placeholderLabel({0, 0}, placeholderText, font, HorizontalAlignment::Center, VerticalAlignment::Center), 
    panel({0, 0}, size, horizontalAlignment, verticalAlignnment, color), 
    UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        userLabel.topLevel = false;
        placeholderLabel.topLevel = false;
        panel.topLevel = false;
        userLabel.MakeInternal(this);
        placeholderLabel.MakeInternal(this);
        panel.MakeInternal(this);
    }

    void EditableLabel::SetPlaceholderText(const char *str)
    {
        placeholderLabel.SetText(str);
    }

    void EditableLabel::SetFont(TTF_Font *font)
    {
        userLabel.SetFont(font);
        placeholderLabel.SetFont(font);
    }

    void EditableLabel::SetFontSize(float size)
    {
        userLabel.SetFontSize(size);
        placeholderLabel.SetFontSize(size);
    }

    void EditableLabel::SetFontColor(Types::Color color)
    {
        userLabel.SetFontColor(color);
    }

    void EditableLabel::SetPlaceholderFontColor(Types::Color color)
    {
        placeholderLabel.SetFontColor(color);
    }

    void EditableLabel::Draw()
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

    void EditableLabel::RemoveCharacter()
    {
        if(userInput.length() == 0)
        {
            return;
        }
        userInput.pop_back();
        userLabel.SetText(userInput.c_str());
    }

    void EditableLabel::KeyPressed(SDL_Keycode key)
    {
        if(key == SDLK_BACKSPACE)
        {
            RemoveCharacter();
        }
    }
    
    void EditableLabel::TextInput(const char *text)
    {
        userInput += text;
        userLabel.SetText(userInput.c_str());
    }
}