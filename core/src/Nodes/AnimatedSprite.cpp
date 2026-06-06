#include "Nodes/AnimatedSprite.hpp"
#include "Renderer/Renderer.hpp"

namespace FWE::Nodes
{
    AnimatedSprite::AnimatedSprite() : Node()
    {
        RegisterVariable("Atlas", Types::Atlas, &atlas);
        RegisterVariable("Tile", Types::Bool, &tile);
        RegisterVariable("Framerate", Types::Float, &framerate);
        RegisterVariable("CurrentFrame", Types::Int, &currentFrame);
        RegisterVariable("FrameCount", Types::Int, &frameCount);
        RegisterVariable("Playing", Types::Bool, &playing);
        lastTime = std::chrono::steady_clock::now();
    }

    void AnimatedSprite::SetCurrentFrame()
    {
        auto currentTime = std::chrono::steady_clock::now();
        
        double delta = std::chrono::duration<double>(currentTime - lastTime).count();
        int framesToAdvance = delta / (1.f / framerate);
        extraTime += (delta / (1.f / framerate)) - framesToAdvance;
        framesToAdvance += (int)extraTime;
        extraTime -= (int)extraTime;

        currentFrame += framesToAdvance;
        currentFrame %= frameCount;

        lastTime = currentTime;
    }

    void AnimatedSprite::Draw()
    {
        if(playing)
        {
            SetCurrentFrame();
        }
        
        const int atlasWidth = atlas.img.width;
        const int atlasHeight = atlas.img.height;

        int startX = atlas.x; 
        int startY = atlas.y; 

        int xPos = (startX + (atlas.width * currentFrame)) % atlasWidth;
        int yPos = (startY + ((startX + (atlas.width * currentFrame)) / atlasWidth) * atlas.height) % atlasHeight;

        glm::vec2 globalPostion = GetGlobalPosition();
        glm::vec2 globalScale = GetGlobalScale();

        glm::vec2 tileCount = {1, 1};
        if(tile)
        {
            tileCount.x = scale.x;
            tileCount.y = scale.y;
        } 

        atlas.x = xPos;
        atlas.y = yPos;

        Renderer::Renderer::GetInstance()->Draw(atlas, globalPostion.x, globalPostion.y, globalScale.x, globalScale.y, tileCount.x, tileCount.y);

        atlas.x = startX;
        atlas.y = startY;
    }
}