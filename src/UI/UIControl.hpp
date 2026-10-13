#pragma once
#include <vector>
#include <memory>

#include "UI/Thickness.hpp"
#include "UI/Rect.hpp"
#include "UI/Size.hpp"

class UIControl
{
public:
    Rect rect;
    Thickness Margin;
    Thickness Padding;

    Size desiredSize;

    virtual ~UIControl() = default;

    virtual void Measure(float availableWidth, float availableHeight) = 0;

    virtual void Arrange(const Rect &finalRect) = 0;

    virtual UIControl *HitTest(float mx, float my)
    {
        if (!rect.IsInside(mx, my))
            return nullptr;
        return this;
    }
};