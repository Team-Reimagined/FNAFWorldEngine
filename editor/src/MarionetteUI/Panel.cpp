#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "Renderer/Renderer.hpp"
#include "Types/Color.hpp"

namespace FWE::MarionetteUI
{
    Panel::Panel(glm::vec2 position, glm::vec2 size, Types::Atlas atlas, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        this->atlas = atlas;
        this->tile = tile;
        this->color = color;
    }

    Panel::Panel(glm::vec2 position, glm::vec2 size, Renderer::Image image, bool tile, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        this->atlas = Types::Atlas::CreateFromImage(image);
        this->tile = tile;
        this->color = color;
    }

    Panel::Panel(glm::vec2 position, glm::vec2 size, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment, Types::Color color) : UIElement(position, size, horizontalAlignment, verticalAlignnment)
    {
        this->color = color;
    }

    void Panel::Draw()
    {
        if(atlas.img.allocatedImg.image != nullptr)
        {
            glm::vec2 offset = GetAlignmentOffset();
            glm::vec2 scale = {size.x / atlas.img.width, size.y / atlas.img.height};
            if(horizontalAlignment == HorizontalAlignment::Full)
            {
                SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
                int windowWitdh;
                SDL_GetWindowSizeInPixels(window, &windowWitdh, NULL);
                scale.x = (float)windowWitdh / atlas.img.width;
            }
            if(verticalAlignnment == VerticalAlignment::Full)
            {
                SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
                int windowHeight;
                SDL_GetWindowSizeInPixels(window, NULL, &windowHeight);
                scale.y = (float)windowHeight / atlas.img.height;
            }
            glm::vec2 tileCount = {1, 1};
            if(tile)
            {
                tileCount = scale;
            }
            Renderer::Renderer::GetInstance()->Draw(atlas, position + offset, scale, tileCount, color);
        }
        else
        {
            glm::vec2 offset = GetAlignmentOffset();
            glm::vec2 scale = size;
            if(horizontalAlignment == HorizontalAlignment::Full)
            {
                SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
                int windowWitdh;
                SDL_GetWindowSizeInPixels(window, &windowWitdh, NULL);
                scale.x = (float)windowWitdh;
            }
            if(verticalAlignnment == VerticalAlignment::Full)
            {
                SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
                int windowHeight;
                SDL_GetWindowSizeInPixels(window, NULL, &windowHeight);
                scale.y = (float)windowHeight;
            }
            Renderer::Renderer::GetInstance()->Draw(position + offset, scale, color);
        }
    }
}