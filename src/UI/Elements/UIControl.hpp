#pragma once
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "UI/Structures/Color.hpp"
#include "UI/Structures/Rect.hpp"
#include "UI/Structures/Size.hpp"
#include "UI/Structures/Thickness.hpp"
#include "UI/UIContext.hpp"

enum class HAlign { Stretch, Left, Center, Right };
enum class VAlign { Stretch, Top, Center, Bottom };

class UIControl
{
public:
    static constexpr float Auto = -1.0f;
    static constexpr float Infinity = std::numeric_limits<float>::infinity();

    std::string Name;

    Thickness Margin;
    Thickness Padding;
    float Width = Auto;
    float Height = Auto;
    HAlign HorizontalAlignment = HAlign::Stretch;
    VAlign VerticalAlignment = VAlign::Stretch;

    Color Background = Color::Transparent();
    bool Visible = true;

    Rect rect;
    Size desiredSize;

    virtual ~UIControl() = default;

    void Measure(float availableWidth, float availableHeight)
    {
        if (!Visible)
        {
            desiredSize = {};
            return;
        }

        const float padW = Padding.Horizontal(), padH = Padding.Vertical();
        const float marW = Margin.Horizontal(), marH = Margin.Vertical();

        const float boxAvailW = (Width != Auto) ? Width : availableWidth - marW;
        const float boxAvailH = (Height != Auto) ? Height : availableHeight - marH;

        const Size content = MeasureOverride({std::max(0.0f, boxAvailW - padW),
                                              std::max(0.0f, boxAvailH - padH)});

        const float boxW = (Width != Auto) ? Width : content.Width + padW;
        const float boxH = (Height != Auto) ? Height : content.Height + padH;
        desiredSize = {boxW + marW, boxH + marH};
    }

    void Arrange(const Rect &slot)
    {
        if (!Visible)
        {
            rect = {slot.x, slot.y, 0, 0};
            return;
        }

        const float availW = std::max(0.0f, slot.w - Margin.Horizontal());
        const float availH = std::max(0.0f, slot.h - Margin.Vertical());

        float boxW = desiredSize.Width - Margin.Horizontal();
        float boxH = desiredSize.Height - Margin.Vertical();

        if (Width == Auto && HorizontalAlignment == HAlign::Stretch)
            boxW = std::max(boxW, availW);
        if (Height == Auto && VerticalAlignment == VAlign::Stretch)
            boxH = std::max(boxH, availH);

        float offX = 0.0f, offY = 0.0f;
        if (HorizontalAlignment == HAlign::Center)
            offX = (availW - boxW) * 0.5f;
        else if (HorizontalAlignment == HAlign::Right)
            offX = availW - boxW;
        if (VerticalAlignment == VAlign::Center)
            offY = (availH - boxH) * 0.5f;
        else if (VerticalAlignment == VAlign::Bottom)
            offY = availH - boxH;

        rect = {slot.x + Margin.Left + offX, slot.y + Margin.Top + offY, boxW, boxH};
        ArrangeOverride(ContentRect());
    }

    virtual void Render(UIContext &ctx)
    {
        if (!Visible)
            return;

        OnRender(ctx);

        for (size_t i = 0; i < ChildCount(); ++i)
            ChildAt(i)->Render(ctx);
    }

    virtual size_t ChildCount() const { return 0; }
    virtual UIControl *ChildAt(size_t) const { return nullptr; }

    virtual UIControl *HitTest(float mx, float my)
    {
        if (!Visible || !rect.IsInside(mx, my))
            return nullptr;
        for (size_t i = ChildCount(); i-- > 0;)
            if (UIControl *hit = ChildAt(i)->HitTest(mx, my))
                return hit;
        return this;
    }

    Rect ContentRect() const
    {
        return {rect.x + Padding.Left, rect.y + Padding.Top,
                std::max(0.0f, rect.w - Padding.Horizontal()),
                std::max(0.0f, rect.h - Padding.Vertical())};
    }

    void DumpTree(int depth = 0) const
    {
        std::cout << std::string(static_cast<size_t>(depth) * 2, ' ') << typeid(*this).name();
        if (!Name.empty())
            std::cout << " \"" << Name << "\"";
        std::cout << "  rect(" << rect.x << ", " << rect.y << ", " << rect.w << " x " << rect.h << ")\n";
        for (size_t i = 0; i < ChildCount(); ++i)
            ChildAt(i)->DumpTree(depth + 1);
    }

protected:
    virtual Size MeasureOverride(Size) { return {}; }

    virtual void ArrangeOverride(const Rect &) {}

    virtual void OnRender(UIContext &) {}
};
