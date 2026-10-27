#pragma once
#include <memory>

#include "Entity/Components/Base/Behaviour.hpp"
#include "UI/Elements.hpp"

class UIDocument : public Behaviour
{
public:
    std::shared_ptr<UIControl> root;

    void UpdateLayout(const Rect &target)
    {
        if (!root)
            return;
        root->Measure(target.w, target.h);
        root->Arrange(target);
    }

    void UpdateLayout(float screenW, float screenH) { UpdateLayout(Rect{0, 0, screenW, screenH}); }

    void Render(UIContext &ctx)
    {
        if (root)
            root->Render(ctx);
    }

    UIControl *HitTest(float x, float y) { return root ? root->HitTest(x, y) : nullptr; }
};
