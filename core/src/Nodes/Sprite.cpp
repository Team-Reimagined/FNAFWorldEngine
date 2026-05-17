#include "Nodes/Sprite.hpp"
#include "Renderer/Renderer.hpp"

namespace FWE::Nodes
{
    Sprite::Sprite() : Node()
    {
        RegisterVariable("Atlas", Types::Atlas, &atlas);
    }

    void Sprite::Draw()
    {
        glm::vec2 globalPostion = GetGlobalPosition();
        glm::vec2 globalScale = GetGlobalScale();
        Renderer::Renderer::GetInstance()->Draw(atlas, globalPostion.x, globalPostion.y, globalScale.x, globalScale.y);
    }
}