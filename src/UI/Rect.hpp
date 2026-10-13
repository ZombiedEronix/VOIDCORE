#pragma once

struct Rect
{
    float x, y;
    float w, h;
    
    bool IsInside(float mx, float my)
    {
        return mx >= x && mx <= x + w &&
               my >= y && my <= y + h;
    }
};
