#pragma once

#include "NodeDatabase.hpp"
#include "Node.hpp"
#include "Sprite.hpp"
#include "AnimatedSprite.hpp"

namespace FWE::Nodes
{
    inline void Initalize()
    {
        NodeDatabase *database = NodeDatabase::GetInstance();
        database->Register<Node>("Node");
        database->Register<Sprite>("Sprite");
        database->Register<AnimatedSprite>("AnimatedSprite");
    }
}