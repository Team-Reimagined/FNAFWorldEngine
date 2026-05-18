#pragma once

#include "Node.hpp"
#include "Types/Atlas.hpp"

namespace FWE::Nodes
{
    class Sprite : public Node
    {
    public:
        Sprite();
        void Draw() override;
    public:
        FWE::Types::Atlas atlas;
        bool tile = false;
    };
}