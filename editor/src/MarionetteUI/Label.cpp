#include "MarionetteUI/Label.hpp"
#include "Renderer/Renderer.hpp"
#include "Types/FontAtlas.hpp"
#include "glm/ext/vector_float2.hpp"

namespace FWE::MarionetteUI
{
    Label::Label(glm::vec2 position, const char *str, Font font, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment) : UIElement(position, {1,1}, horizontalAlignment, verticalAlignnment)
    {
        blockMouse = false;

        this->font = font;

        SetText(str);
    }

    void Label::SetText(const char *str)
    {
        text = str;
        if(text == "")
        {
            text = " ";
        }
        SetTextSize();
    }

    void Label::SetFont(Types::FontAtlas *font)
    {
        this->font.fontAtlas = font;
        SetTextSize();
    }

    void Label::SetFontSize(float size)
    {
        font.fontSize = size;
        SetTextSize();
    }

    void Label::SetFontColor(Types::Color color)
    {
        font.color = color;
    }

    void Label::SetTextSize()
    {
        const char *currentChar = text.c_str();
        double up, down, left, right;
        font.fontAtlas->layout[*currentChar - 32].getQuadPlaneBounds(left, down, right, up);
        float leftBound = left;
            
        float cursor = 0;
        while(*(currentChar + 1) != '\0')
        {
            cursor += font.fontAtlas->layout[*currentChar - 32].getAdvance();
            currentChar++;
        }

            
        font.fontAtlas->layout[*currentChar - 32].getQuadPlaneBounds(left, down, right, up);
        float rightBound = cursor + right;
            
        float horizontalSize = (rightBound - leftBound) * font.fontSize;

        font.fontAtlas->layout['A' - 32].getQuadPlaneBounds(left, down, right, up);
        float verticalSize = (up - down) * font.fontSize;

        size = {horizontalSize, verticalSize};
    }

    void Label::Draw()
    {
        glm::vec2 offset = GetAlignmentOffset();
        Renderer::Renderer::GetInstance()->DrawFont(font.fontAtlas, text.c_str(), GetGlobalPosition() + offset, font.fontSize, font.color);
    }

    glm::vec2 Label::GetTextSize()
    {
        return size;
    }
}
