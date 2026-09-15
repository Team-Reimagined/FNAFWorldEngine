#include "MarionetteUI/UIElement.hpp"
#include "MarionetteUI/UIManager.hpp"
#include "Util/Logging.hpp"
#include "Renderer/Renderer.hpp"
#include <algorithm>

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
        if(parent != nullptr)
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
        ReleaseFocus();
    }

    UIElement::UIElement(UIElement &&other) noexcept :
    children(std::move(other.children)),
    internalChildren(std::move(other.internalChildren))
    {
        for(auto &i : children)
        {
            i->parent = this;
        }

        for(auto &i : internalChildren)
        {
            i->parent = this;
        }

        position = other.position;
        size = other.size;
        visible = other.visible;
        blockMouse = other.blockMouse;
        horizontalAlignment = other.horizontalAlignment;
        verticalAlignnment = other.verticalAlignnment;
        topLevel = other.topLevel;
        internal = other.internal;

        if(other.parent != nullptr)
        {
            if(internal)
            {
                MakeInternal(other.parent);
            }
            else
            {
                other.parent->AddChild(this);
            }
        }
        other.RemoveFromTree();
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
        children[index]->parent = nullptr;
        children.erase(children.begin() + index);
    }

    void UIElement::RemoveFromTree()
    {
        if(parent == nullptr)
        {
            return;
        }
        if(internal)
        {
            parent = nullptr;
            return;
        }
        for(int i = 0; i < parent->GetChildrenCount(); i++)
        {
            if(parent->GetChild(i) == this)
            {
                parent->RemoveChild(i);
                break;
            }
        }
    }

    glm::vec2 UIElement::GetGlobalPosition()
    {
        if(!topLevel && parent != nullptr)
        {
            return position + parent->GetGlobalPosition();
        }
        return position;
    }

    glm::vec2 UIElement::GetAlignmentOffset()
    {
        SDL_Window *window = Renderer::Renderer::GetInstance()->GetWindow();
        int windowWidth;
        int windowHeight;
        SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);

        if(horizontalAlignment == HorizontalAlignment::Full)
        {
            size.x = windowWidth;
        }
        if(verticalAlignnment == VerticalAlignment::Full)
        {
            size.y = windowHeight;
        }

        glm::vec2 offset = {0, 0};

        switch (horizontalAlignment)
        {
        case HorizontalAlignment::Left:
            if(topLevel)
            {
                offset.x = size.x / 2.;
            }
            else
            {
                offset.x = GetParent()->GetAlignmentOffset().x - GetParent()->size.x / 2. + size.x / 2.;
            }
            break;
        case HorizontalAlignment::Center:
        case HorizontalAlignment::Full:
            if(topLevel)
            {
                offset.x = windowWidth / 2.;
            }
            else
            {
                offset.x = GetParent()->GetAlignmentOffset().x;
            }
            break;
        case HorizontalAlignment::Right:
            if(topLevel)
            {
                offset.x = windowWidth - size.x / 2.;
            }
            else
            {
                offset.x = GetParent()->GetAlignmentOffset().x + GetParent()->size.x / 2. - size.x / 2.;;
            }
            break;
        default:
            break;
        }

        switch (verticalAlignnment)
        {
        case VerticalAlignment::Top:
            if(topLevel)
            {
                offset.y = size.y / 2.;
            }
            else
            {
                offset.y = GetParent()->GetAlignmentOffset().y - GetParent()->size.y / 2. + size.y / 2.;
            }
            break;
        case VerticalAlignment::Center:
        case VerticalAlignment::Full:
            if(topLevel)
            {
                offset.y = windowHeight / 2.;
            }
            else
            {
                offset.y = GetParent()->GetAlignmentOffset().y;
            }
            break;
        case VerticalAlignment::Bottom:
            if(topLevel)
            {
                offset.y = windowHeight - size.y / 2.;
            }
            else
            {
                offset.y = GetParent()->GetAlignmentOffset().y + GetParent()->size.y / 2 - size.y / 2.;
            }
            break;
        default:
            break;
        }

        return offset;
    }
    
    void UIElement::MakeInternal(UIElement *parent)
    {
        if(this->parent == nullptr)
        {
            this->parent = parent;
            internal = true;
            parent->AddInternalChild(this);
        }
        else
        {
            Util::Logging::error("UIElement is already a child of another UIElement");
        }
    }

    void UIElement::AddInternalChild(UIElement *element)
    {
        if(std::find(internalChildren.begin(), internalChildren.end(), element) == internalChildren.end())
        {
            internalChildren.push_back(element);
        }
    }

    void UIElement::GrabFocus()
    {
        UIManager::GetInstance()->GrabFocus(this);
    }

    void UIElement::ReleaseFocus()
    {
        UIManager::GetInstance()->ReleaseFocus(this);
    }
}