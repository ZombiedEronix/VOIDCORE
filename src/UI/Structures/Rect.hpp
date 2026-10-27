#pragma once

struct Rect
{
    float x = 0.0f, y = 0.0f;
    float w = 0.0f, h = 0.0f;

    float Right() const { return x + w; }
    float Bottom() const { return y + h; }

    bool IsInside(float mx, float my) const
    {
        return mx >= x && mx <= x + w &&
               my >= y && my <= y + h;
    }
};
