#pragma once

#include "MarionetteUI/Button.hpp"
#include "MarionetteUI/Font.hpp"
#include "MarionetteUI/Panel.hpp"
#include "Types/Color.hpp"
#include "glm/ext/vector_float2.hpp"
#include <functional>
#include <vector>
namespace FWE::Editor
{
    class RightClickPanel : public MarionetteUI::Panel
    {
    public:
        RightClickPanel(glm::vec2 buttonSize, MarionetteUI::Font font, Types::Color color = 0xFFFFFFFF);
        void ShowPanel(glm::vec2 pos);

        void OnFocusLost() override;

        void AddOption(const char *text, std::function<void()> callback);
    private:
        std::vector<MarionetteUI::Button> options;
        glm::vec2 buttonSize;
        MarionetteUI::Font font;
    };
}