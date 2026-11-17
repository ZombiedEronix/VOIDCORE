#pragma once
#include <cstdint>

enum class ShaderPropertyType : uint8_t
{
    Float = 0,
    Vec2 = 1,
    Vec4 = 2,
    Color = 3,
    Int = 4,
    Tex2D = 5
};
