#pragma once

#include "Thickness.hpp"

struct UIVertex
{
    float pos[2];
    float uv[2];
    float rect[4];
};
static_assert(sizeof(UIVertex) == 32, "UIVertex layout is used by UIPass");
