#pragma once
#include <algorithm>

#include "UI/Elements/UIControl.hpp"

class ProgressBar : public UIControl
{
public:
    float Value = 0.0f;
    Color Fill = Color(0.30f, 0.80f, 0.40f);

    ProgressBar()
    {
        Height = 14.0f;
        Background = Color(0.20f, 0.20f, 0.25f);
    }

protected:
    Size MeasureOverride(Size) override { return {120.0f, 14.0f}; }

    void OnRender(UIContext &ctx) override
    {
        // Rect r = ContentRect();
        // r.w *= std::clamp(Value, 0.0f, 1.0f);
        // ctx.AddRect(r, Fill);
    }
};
