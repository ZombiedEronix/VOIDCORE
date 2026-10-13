#pragma once

#include "UI/UIControl.hpp"
#include "UI/Thickness.hpp"
#include "UI/Color.hpp"

class Panel : public UIControl
{
public:
    std::vector<std::shared_ptr<UIControl>> children;

    void Measure(float availableWidth, float availableHeight) override
    {
        float maxW = 0.0f;
        float maxH = 0.0f;

        float innerW = availableWidth - (Padding.Left + Padding.Right);
        float innerH = availableHeight - (Padding.Top + Padding.Bottom);

        for (auto &child : children)
        {
            child->Measure(innerW, innerH);

            maxW = std::max(maxW, child->desiredSize.Width);
            maxH = std::max(maxH, child->desiredSize.Height);
        }

        desiredSize.Width = maxW + Padding.Left + Padding.Right;
        desiredSize.Height = maxH + Padding.Top + Padding.Bottom;
    }

    void Arrange(const Rect &finalRect) override
    {
        rect = finalRect;

        Rect innerRect;
        innerRect.x = rect.x + Padding.Left;
        innerRect.y = rect.y + Padding.Top;
        innerRect.w = rect.w - (Padding.Left + Padding.Right);
        innerRect.h = rect.h - (Padding.Top + Padding.Bottom);

        for (auto &child : children)
        {
            child->Arrange(innerRect);
        }
    }
};