#include "MarionetteUI/UIManager.hpp"
#include "Util/Logging.hpp"
#include "Input/InputManager.hpp"
#include "MarionetteUI/Button.hpp"
#include "MarionetteUI/TextInput.hpp"
#include "Renderer/Renderer.hpp"

namespace FWE::MarionetteUI
{
    void UIManager::Init()
    {
        if(initalized)
        {
            return;
        }
        TTF_Init();
        SDL_StartTextInput(Renderer::Renderer::GetInstance()->GetWindow());
        Input::InputManager::GetInstance()->AddInputCallback([](const SDL_Event *event){UIManager::GetInstance()->ProccessInputEvent(event);});
        root.blockMouse = false;
        initalized = true;
    }

    void UIManager::Shutdown()
    {
        for(auto &[str, font] : fonts)
        {
            TTF_CloseFont(font);
        }
        fonts.clear();
        TTF_Quit();
        SDL_StopTextInput(Renderer::Renderer::GetInstance()->GetWindow());
    }

    TTF_Font *UIManager::LoadFont(const char *filePath)
    {
        if(fonts.find(filePath) != fonts.end())
        {
            return fonts.at(filePath);
        }
        else
        {
            TTF_Font *font = TTF_OpenFont(filePath, 12);
            fonts.insert({filePath, font});
            return font;
        }
    }

    template <typename T> bool IsElementType(UIElement *element)
    {
        if(dynamic_cast<T *>(element) != nullptr)
        {
            return true;
        }
        return false;
    }

    void UIManager::ProccessInputEvent(const SDL_Event *event)
    {
        if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT)
        {
            float x, y;
            SDL_GetMouseState(&x, &y);
            CheckElementClicked({x, y});
        }
        else if(IsElementType<TextInput>(selected))
        {
            if(event->type == SDL_EVENT_TEXT_INPUT)
            {
                ((TextInput *)selected)->AddCharacter(event->text.text);
            }
            else if(event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_BACKSPACE)
            {
                ((TextInput *)selected)->RemoveCharacter();
            }
        }
    }

    void CheckElementRecursive(glm::vec2 mousePos, UIElement **elementClicked, UIElement *element)
    {
        if(element->visible)
        {
            glm::vec2 offset = element->GetAlignmentOffset();
            const float leftBound = element->position.x + offset.x - element->size.x / 2;
            const float rightBound = element->position.x + offset.x + element->size.x / 2;
            const float upBound = element->position.y + offset.y - element->size.y / 2;
            const float downBound = element->position.y + offset.y + element->size.y / 2;
            if(mousePos.x >= leftBound && mousePos.x <= rightBound && mousePos.y >= upBound && mousePos.y <= downBound && element->blockMouse)
            {
                *elementClicked = element;
            }
            for(int i = 0; i < element->GetChildrenCount(); i++)
            {
                CheckElementRecursive(mousePos, elementClicked, element->GetChild(i));
            }
        }
    }

    void UIManager::CheckElementClicked(glm::vec2 mousePos)
    {
        selected = nullptr;
        CheckElementRecursive(mousePos, &selected, &root);
        if(IsElementType<Button>(selected))
        {
            ((Button *)selected)->Pressed();
        }
    }

    void UIManager::AddUIElementToTree(UIElement *element)
    {
        root.AddChild(element);
    }

    void DrawRecursive(UIElement *element)
    {
        if(element->visible)
        {
            element->Draw();
            for(int i = 0; i < element->GetChildrenCount(); i++)
            {
                DrawRecursive(element->GetChild(i));
            }
        }
    }

    void UIManager::Draw()
    {
        DrawRecursive(&root);  
    }
}