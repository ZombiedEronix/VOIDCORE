#pragma once

#include "Thickness.hpp"

struct UIVertex
{
    float pos[2];
    float uv[2];
    float rect[4];
    //============
    float color[4];
    float cornerSize[4];
    //============
    float borderColor[4];
    float borderThickness[4];
    //============
};
static_assert(sizeof(UIVertex) == 96, "UIVertex layout is used by UIPass");
