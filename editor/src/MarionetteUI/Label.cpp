#include "MarionetteUI/Label.hpp"
#include "Renderer/Renderer.hpp"

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
        Generate();
    }

    void Label::SetFont(TTF_Font *font)
    {
        this->font.font = font;
        Generate();
    }

    void Label::SetFontSize(float size)
    {
        font.fontSize = size;
        Generate();
    }

    void Label::SetFontColor(Types::Color color)
    {
        font.color = color;
        Generate();
    }

    void Label::Generate()
    {
        TTF_SetFontSize(font.font, font.fontSize);
        SDL_Surface *surfArgb = TTF_RenderText_Blended(font.font, text.c_str(), 0, {font.color.r, font.color.g, font.color.b, font.color.a});
        SDL_Surface *surf = SDL_ConvertSurface(surfArgb, SDL_PIXELFORMAT_RGBA32);
        SDL_DestroySurface(surfArgb);
        Renderer::Renderer *renderer = Renderer::Renderer::GetInstance();
        if(texture.img.id != -1)
        {
            renderer->RemoveImage(texture.img);
        }
        texture.img = 
        {
            .width = (uint32_t)surf->w,
            .height = (uint32_t)surf->h
        };

        texture.width = texture.img.width;
        texture.height = texture.img.height;

        size.x = texture.width;
        size.y = texture.height;

        texture.img.id = renderer->AddImage({surf->pixels, texture.img});
        SDL_DestroySurface(surf);
    }

    void Label::Draw()
    {
        glm::vec2 offset = GetAlignmentOffset();
        Renderer::Renderer::GetInstance()->Draw(texture, position + offset, {size.x / texture.img.width, size.y / texture.img.height});
    }

    glm::vec2 Label::GetTextSize()
    {
        return {texture.width, texture.height};
    }
}
