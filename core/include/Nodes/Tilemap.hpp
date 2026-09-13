#pragma once

#include "Nodes/Node.hpp"

namespace FWE::Nodes
{
    class Tilemap : public Node
    {
    public:
        void Draw() override;
    public:
        FWE::Types::Atlas atlas;
    };
}