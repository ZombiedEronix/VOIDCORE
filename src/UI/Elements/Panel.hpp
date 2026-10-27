#pragma once
#include <type_traits>
#include "UI/Elements/UIControl.hpp"

class Panel : public UIControl
{
public:
    std::vector<std::shared_ptr<UIControl>> children;

    template <typename T>
    std::shared_ptr<T> Add(std::shared_ptr<T> child)
    {
        static_assert(std::is_base_of_v<UIControl, T>, "child must derive from UIControl");
        children.push_back(child);
        return child;
    }

    size_t ChildCount() const override { return children.size(); }
    UIControl *ChildAt(size_t i) const override { return children[i].get(); }

protected:
    Size MeasureOverride(Size available) override
    {
        Size result;
        for (auto &child : children)
        {
            child->Measure(available.Width, available.Height);
            result.Width = std::max(result.Width, child->desiredSize.Width);
            result.Height = std::max(result.Height, child->desiredSize.Height);
        }
        return result;
    }

    void ArrangeOverride(const Rect &content) override
    {
        for (auto &child : children)
            child->Arrange(content);
    }
};
