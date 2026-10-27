#pragma once

#include "Thickness.hpp"

struct UIVertex
{
    float pos[2];
    float uv[2];
    Color color;
    float size[2];
    float corners[4];
    float bounds[4];
    Color bColor;

    UIVertex(float x, float y, float u, float v, const Color &c,
             float w, float h, const Thickness &corners, const Rect &bounds,
             const Color &borderColor = Color::Transparent())
        : pos{x, y}, uv{u, v}, color(c), size{w, h}, corners{corners.Left, corners.Top, corners.Right, corners.Bottom}, bounds{bounds.x, bounds.y, bounds.Right(), bounds.Bottom()}, bColor(borderColor)
    {
    }
};
static_assert(sizeof(UIVertex) == 88, "UIVertex layout is used by UIPass");
