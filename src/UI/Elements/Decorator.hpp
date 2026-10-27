#pragma once
#include "UI/Elements/UIControl.hpp"

class Decorator : public UIControl
{
public:
    std::shared_ptr<UIControl> child;

    Thickness CornerRadius;
    Color BorderColor = Color::Transparent();
    float BorderThickness = 0.0f;

    size_t ChildCount() const override { return child ? 1 : 0; }
    UIControl *ChildAt(size_t) const override { return child.get(); }

protected:
    Size MeasureOverride(Size available) override
    {
        if (!child)
            return {};
        child->Measure(available.Width, available.Height);
        return child->desiredSize;
    }

    void ArrangeOverride(const Rect &content) override
    {
        if (child)
            child->Arrange(content);
    }

    void OnRender(UIContext &ctx) override
    {
        if (Background.IsVisible())
            ctx.AddRect(rect, Background, CornerRadius);
    }
};
