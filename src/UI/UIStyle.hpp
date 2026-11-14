#pragma once
#include <string>
#include <vector>
#include "UI/Elements/UIControl.hpp"

struct StyleSetter
{
    std::string property;
    std::string value;
};

struct UIStyle
{
    std::string targetType;
    std::string targetClass;
    std::vector<StyleSetter> setters;

    bool Matches(const UIControl *ctrl) const
    {
        if (!targetType.empty() && ctrl->GetTypeName() != targetType)
            return false;
        if (!targetClass.empty())
        {
            auto &c = ctrl->Classes;
            if (std::find(c.begin(), c.end(), targetClass) == c.end())
                return false;
        }
        return true;
    }
};
