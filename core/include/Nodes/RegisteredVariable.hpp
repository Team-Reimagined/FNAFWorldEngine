#pragma once

#include "External/json.hpp"
#include "Types/Variant.hpp"

namespace FWE::Nodes
{
    enum Types
    {
        Int,
        Float,
        Bool,
        String,
        Vector2,
        Atlas
    };

    struct RegisteredVariable
    {
        Types type;

        RegisteredVariable(Types type, void *var) : type(type), variable(var) {}

        void SetVariable(const FWE::Types::Variant &value);
        void SetVariable(const nlohmann::json::value_type &value);
        FWE::Types::Variant GetVariable();
    private:
        void *variable;
    };
}