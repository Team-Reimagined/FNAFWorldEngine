#pragma once

#include "Types/Atlas.hpp"
#include "glm/ext/vector_float2.hpp"
#include <string>
#include <variant>

namespace FWE::Types
{
    using Variant = std::variant<int, float, bool, std::string, glm::vec2, Types::Atlas>;
}