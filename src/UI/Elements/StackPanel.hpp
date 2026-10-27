#pragma once
#include "UI/Elements/Panel.hpp"

enum class Orientation { Vertical, Horizontal };

// Выстраивает детей в столбик или в строку.
// Поперёк оси дети растягиваются (Stretch) или выравниваются по своему Alignment.
class StackPanel : public Panel
{
public:
    Orientation orientation = Orientation::Vertical;
    float Spacing = 0.0f; // промежуток между детьми

    StackPanel() = default;
    explicit StackPanel(Orientation o, float spacing = 0.0f) : orientation(o), Spacing(spacing) {}

protected:
    Size MeasureOverride(Size available) override
    {
        const bool vertical = orientation == Orientation::Vertical;
        float along = 0.0f, across = 0.0f;
        int shown = 0;

        for (auto &child : children)
        {
            if (!child->Visible)
                continue;
            // Вдоль оси ребёнку отдаём «бесконечность»: пусть скажет, сколько ему надо.
            child->Measure(vertical ? available.Width : Infinity,
                           vertical ? Infinity : available.Height);
            const Size d = child->desiredSize;
            along += vertical ? d.Height : d.Width;
            across = std::max(across, vertical ? d.Width : d.Height);
            ++shown;
        }
        if (shown > 1)
            along += Spacing * static_cast<float>(shown - 1);

        return vertical ? Size{across, along} : Size{along, across};
    }

    void ArrangeOverride(const Rect &content) override
    {
        const bool vertical = orientation == Orientation::Vertical;
        float pos = vertical ? content.y : content.x;

        for (auto &child : children)
        {
            if (!child->Visible)
                continue;
            const Size d = child->desiredSize;
            if (vertical)
            {
                child->Arrange({content.x, pos, content.w, d.Height});
                pos += d.Height + Spacing;
            }
            else
            {
                child->Arrange({pos, content.y, d.Width, content.h});
                pos += d.Width + Spacing;
            }
        }
    }
};
