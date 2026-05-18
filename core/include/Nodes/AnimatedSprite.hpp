#pragma once

#include "Node.hpp"
#include "Types/Atlas.hpp"
#include <vector>
#include <chrono>

namespace FWE::Nodes
{
    class AnimatedSprite : public Node
    {
    public:
        AnimatedSprite();
        void Draw() override;
    private:
        void SetCurrentFrame();
    public:
        FWE::Types::Atlas atlas = {};
        float framerate = 30;
        int frameCount = 1;
        int currentFrame = 0;
        bool playing = true;
        bool tile = false;
    private:
        std::chrono::_V2::system_clock::time_point lastTime;
        double extraTime = 0;
    };
}