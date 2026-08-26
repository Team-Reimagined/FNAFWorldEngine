#include "Nodes/RegisteredVariable.hpp"
#include "ResourceLoader/ImageLoader.hpp"
#include "Types/Atlas.hpp"
#include "Types/Variant.hpp"
#include "glm/ext/vector_float2.hpp"
#include <string>

namespace FWE::Nodes
{
    void RegisteredVariable::SetVariable(const FWE::Types::Variant &value)
    {
        switch (type)
        {
        case Int:
        {
            const int *ptr = std::get_if<int>(&value);
            if(ptr != nullptr)
            {
                *(int *)variable = *ptr;
            }
            break;
        }
        case Float:
        {
            const float *ptr = std::get_if<float>(&value);
            if(ptr != nullptr)
            {
                *(float *)variable = *ptr;
            }
            break;
        }
        case Bool:
        {
            const bool *ptr = std::get_if<bool>(&value);
            if(ptr != nullptr)
            {
                *(bool *)variable = *ptr;
            }
            break;
        }
        case String:
        {
            const std::string *ptr = std::get_if<std::string>(&value);
            if(ptr != nullptr)
            {
                *(std::string *)variable = *ptr;
            }
            break;
        }
        case Vector2:
        {
            const glm::vec2 *ptr = std::get_if<glm::vec2>(&value);
            if(ptr != nullptr)
            {
                *(glm::vec2 *)variable = *ptr;
            }
            break;
        }
        case Atlas:
        {
            const FWE::Types::Atlas *ptr = std::get_if<FWE::Types::Atlas>(&value);
            if(ptr != nullptr)
            {
                *(FWE::Types::Atlas *)variable = *ptr;
            }
            break;
        }
        }
    }

    void RegisteredVariable::SetVariable(const nlohmann::json::value_type &value)
    {
        switch (type)
        {
        case Int:
        {
            *(int *)variable = value;
            break;
        }
        case Float:
        {
            *(float *)variable = value;
            break;
        }
        case Bool:
        {
            *(bool *)variable = value;
            break;
        }
        case String:
        {
            *(std::string *)variable = value;
            break;
        }
        case Vector2:
        {
            glm::vec2 vec2 = {value[0], value[1]};
            *(glm::vec2 *)variable = vec2;
            break;
        }
        case Atlas:
        {
            ResourceLoader::ImageLoader *loader = ResourceLoader::ImageLoader::GetInstance();
            FWE::Types::Atlas atlas = {loader->LoadImage(((std::string)value[0]).c_str()), value[1], value[2], value[3], value[4]};
            *(FWE::Types::Atlas *)variable = atlas;
            break;
        }
        }
    }
    
    FWE::Types::Variant RegisteredVariable::GetVariable()
    {
        switch (type)
        {
        case Int:
            return *(int *)variable;
        case Float:
            return *(float *)variable;
        case Bool:
            return *(bool *)variable;
        case String:
            return *(std::string *)variable;
        case Vector2:
            return *(glm::vec2 *)variable;
        case Atlas:
            return *(FWE::Types::Atlas *)variable;
        }
    }
}