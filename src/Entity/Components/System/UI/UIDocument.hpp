#pragma once
#include <memory>

#include "Entity/Components/Base/Behaviour.hpp"
#include "UI/Elements.hpp"

class UIDocument : public Behaviour
{
public:
    std::shared_ptr<UIControl> root;

    UIControl *m_hoveredControl = nullptr;
    UIControl *m_pressedControl = nullptr;

    void Update() override
    {
        if (!root || !IsEnabled())
            return;

        glm::vec2 mousePos = Input::GetMousePosition();
        UIControl *currentHover = root->HitTest(mousePos.x, mousePos.y);

        if (currentHover != m_hoveredControl)
        {
            if (m_hoveredControl)
            {
                m_hoveredControl->OnMouseLeave();
            }

            m_hoveredControl = currentHover;

            if (m_hoveredControl)
            {
                m_hoveredControl->OnMouseEnter();
            }
        }

        if (Input::IsMouseButtonHeld(1) && m_hoveredControl)
        {
            m_pressedControl = m_hoveredControl;
            m_pressedControl->OnMouseDown();
        }

        if (!Input::IsMouseButtonHeld(1) && m_pressedControl)
        {
            m_pressedControl->OnMouseUp();

            if (m_pressedControl == m_hoveredControl)
            {
                m_pressedControl->OnClick();
            }

            m_pressedControl = nullptr;
        }
    }

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
