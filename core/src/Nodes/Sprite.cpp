#include "Nodes/Sprite.hpp"
#include "Renderer/Renderer.hpp"

namespace FWE::Nodes
{
    Sprite::Sprite() : Node()
    {
        RegisterVariable("Atlas", Types::Atlas, &atlas);
        RegisterVariable("Tile", Types::Bool, &tile);
    }

    void Sprite::Draw()
    {
        glm::vec2 globalPostion = GetGlobalPosition();
        glm::vec2 globalScale = GetGlobalScale();
        glm::vec2 tileCount = {1, 1};
        if(tile)
        {
            tileCount.x = scale.x;
            tileCount.y = scale.y;
        } 
        Renderer::Renderer::GetInstance()->Draw(atlas, globalPostion, globalScale, tileCount);
    }
}