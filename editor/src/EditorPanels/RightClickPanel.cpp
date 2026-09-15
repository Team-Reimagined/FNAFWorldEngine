#include "EditorPanels/RightClickPanel.hpp"
#include "MarionetteUI/Font.hpp"
#include "MarionetteUI/Panel.hpp"
#include "MarionetteUI/UIElement.hpp"
#include "Types/Color.hpp"
#include "glm/ext/vector_float2.hpp"

namespace FWE::Editor
{
    RightClickPanel::RightClickPanel(glm::vec2 buttonSize, MarionetteUI::Font font, Types::Color color) :
    MarionetteUI::Panel({0,0}, buttonSize, MarionetteUI::HorizontalAlignment::Left, MarionetteUI::VerticalAlignment::Top, color)
    {
        visible = false;
        this->buttonSize = buttonSize;
        this->font = font;
    }

    void RightClickPanel::ShowPanel(glm::vec2 pos)
    {
        position = pos;
        visible = true;
        GrabFocus();
    }

    void RightClickPanel::OnFocusLost()
    {
        visible = false;
    }

    void RightClickPanel::AddOption(const char *text, std::function<void()> callback)
    {
        options.emplace_back(glm::vec2{0, options.size() * buttonSize.y}, buttonSize, text, font, MarionetteUI::HorizontalAlignment::Left, MarionetteUI::VerticalAlignment::Top, color);
        options.back().SetCallback(callback);
        AddChild(&options.back());
        options.back().topLevel = false;
        size.y = buttonSize.y * options.size();
    }
}