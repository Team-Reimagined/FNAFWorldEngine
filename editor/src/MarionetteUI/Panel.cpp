#include "MarionetteUI/Panel.hpp"
#include "Renderer/Renderer.hpp"

namespace FWE::MarionetteUI
{
    Panel::Panel(glm::vec2 position, glm::vec2 size, Types::Atlas atlas, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment) : UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        this->atlas = atlas;
        this->tile = tile;
    }

    Panel::Panel(glm::vec2 position, glm::vec2 size, Renderer::Image image, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment) : UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        this->atlas = Types::Atlas::CreateFromImage(image);
        this->tile = tile;
    }

    void Panel::Draw()
    {
        glm::vec2 tileCount = {1, 1};
        if(tile)
        {
            tileCount.x = size.x / atlas.img.width;
            tileCount.y = size.y / atlas.img.height;
        }
        glm::vec2 offset = GetAlignmentOffset();
        glm::vec2 scale = {size.x / atlas.img.width, size.y / atlas.img.height};
        if(horizontalAlignment == FullHorizontal)
        {
            SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
            int windowWitdh;
            SDL_GetWindowSizeInPixels(window, &windowWitdh, NULL);
            scale.x = (float)windowWitdh / atlas.img.width;
        }
        if(verticalAlignnment == FullVertical)
        {
            SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
            int windowHeight;
            SDL_GetWindowSizeInPixels(window, NULL, &windowHeight);
            scale.y = (float)windowHeight / atlas.img.height;
        }
        Renderer::Renderer::GetInstance()->Draw(atlas, position.x + offset.x, position.y + offset.y, scale.x, scale.y);
    }
}