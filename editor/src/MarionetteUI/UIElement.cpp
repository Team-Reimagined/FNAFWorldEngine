#include "MarionetteUI/UIElement.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Util/Logging.hpp"
#include "Renderer/Renderer.hpp"

namespace FWE::MarionetteUI
{
    UIElement::UIElement(glm::vec2 position, glm::vec2 size, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignnment)
    {
        this->position = position;
        this->size = size;
        this->horizontalAlignment = horizontalAlignment;
        this->verticalAlignnment = verticalAlignnment;
    }

    UIElement::~UIElement()
    {
        for(int i = 0; i < parent->GetChildrenCount(); i++)
        {
            if(parent->GetChild(i) == this)
            {
                parent->RemoveChild(i);
                break;
            }
        }
    }

    UIElement *UIElement::GetParent()
    {
        return parent;
    }

    void UIElement::AddChild(UIElement *element)
    {
        if(element->parent == nullptr)
        {
            element->parent = this;
            children.push_back(element);
        }
        else
        {
            Util::Logging::error("UIElement is already a child of another UIElement");
        }
    }

    UIElement *UIElement::GetChild(unsigned int index)
    {
        if(index >= children.size())
        {
            return nullptr;
        }
        return children[index];
    }

    std::vector<UIElement *> &UIElement::GetChildren()
    {
        return children;
    }

    unsigned int UIElement::GetChildrenCount()
    {
        return children.size();
    }

    void UIElement::RemoveChild(unsigned int index)
    {
        if(index >= children.size())
        {
            return;
        }
        children.erase(children.begin() + index);
    }

    void UIElement::RemoveFromTree()
    {
        for(int i = 0; i < parent->GetChildrenCount(); i++)
        {
            if(parent->GetChild(i) == this)
            {
                parent->RemoveChild(i);
                break;
            }
        }
    }

    glm::vec2 UIElement::GetAlignmentOffset()
    {
        glm::vec2 offset = {0, 0};

        switch (horizontalAlignment)
        {
        case Left:
            offset.x = size.x / 2.;
            break;
        case Center:
        case FullHorizontal:
        {
            SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
            int windowWidth;
            SDL_GetWindowSizeInPixels(window, &windowWidth, NULL);
            offset.x = windowWidth / 2;
            break;
        }
        case Right:
        {
            SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
            int windowWidth;
            SDL_GetWindowSizeInPixels(window, &windowWidth, NULL);
            offset.x = windowWidth - size.x / 2.;
            break;
        }
        default:
            break;
        }

        switch (verticalAlignnment)
        {
        case Top:
            offset.y = size.y / 2.;
            break;
        case Middle:
        case FullVertical:
        {
            SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
            int windowHeight;
            SDL_GetWindowSizeInPixels(window, NULL, &windowHeight);
            offset.y = windowHeight / 2;
            break;
        }
        case Bottom:
        {
            SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
            int windowHeight;
            SDL_GetWindowSizeInPixels(window, NULL, &windowHeight);
            offset.y = windowHeight - size.y / 2.;
            break;
        }
        default:
            break;
        }

        return offset;
    }
}