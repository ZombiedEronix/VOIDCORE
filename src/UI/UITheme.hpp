#pragma once
#include <vector>
#include "UIStyle.hpp"
#include "UI/Elements/UIControl.hpp"
class UITheme
{
public:
    static void AddStyle(const UIStyle &style)
    {
        styles.push_back(style);
    }

    static void Apply(UIControl *ctrl)
    {
        if (!ctrl)
            return;

        for (const auto &style : styles)
        {
            if (style.Matches(ctrl))
            {
                for (const auto &setter : style.setters)
                {
                    ctrl->SetProperty(setter.property, setter.value);
                }
            }
        }
    }

private:
    static inline std::vector<UIStyle> styles;
};
