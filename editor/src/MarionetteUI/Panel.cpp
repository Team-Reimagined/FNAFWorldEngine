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
            glm::vec2 tileCount = {1, 1};
            if(tile)
            {
                tileCount = scale;
            }
            Renderer::Renderer::GetInstance()->Draw(atlas, GetGlobalPosition() + offset, scale, tileCount, color);
        }
        else
        {
            glm::vec2 offset = GetAlignmentOffset();
            Renderer::Renderer::GetInstance()->Draw(GetGlobalPosition() + offset, size, color);
        }
    }
}