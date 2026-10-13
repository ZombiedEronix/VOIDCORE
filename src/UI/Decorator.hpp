#pragma once
#include "UI/UIControl.hpp"
#include "UI/Thickness.hpp"
#include "UI/Color.hpp"

class Decorator : public UIControl
{
public:
    std::shared_ptr<UIControl> child;

    void Measure(float availableWidth, float availableHeight) override
    {
        if (child)
        {
            float childAvailW = availableWidth - (Padding.Left + Padding.Right);
            float childAvailH = availableHeight - (Padding.Top + Padding.Bottom);

            child->Measure(childAvailW, childAvailH);

            desiredSize.Width = child->desiredSize.Width + Padding.Left + Padding.Right;
            desiredSize.Width = child->desiredSize.Width + Padding.Top + Padding.Bottom;
        }
    }

    void Arrange(const Rect &finalRect) override
    {
        rect = finalRect;

        if (child)
        {
            Rect childRect;
            childRect.x = rect.x + Padding.Left;
            childRect.y = rect.y + Padding.Top;
            childRect.w = rect.w - (Padding.Left + Padding.Right);
            childRect.h = rect.h - (Padding.Top + Padding.Bottom);

            child->Arrange(childRect);
        }
    }
};