#pragma once

#include <bit>
#include <cstdint>

namespace FWE::Types
{
    union Color
    {
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

        uint32_t color;

        Color() : color(0xFFFFFFFF) {}
        Color(uint32_t value) : color(std::byteswap(value)) {}
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
    };
}