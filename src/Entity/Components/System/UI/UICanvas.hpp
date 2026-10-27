#pragma once
#include "Entity/Components/Base/Behaviour.hpp"
#include "Entity/Components/System/UI/UIDocument.hpp"
#include "UI/Elements.hpp"

// Куда и в каком порядке рисовать документ. Сейчас — только Screen Space.
class UICanvas : public Behaviour
{
public:
    int sortingOrder = 0; // больше = рисуется позже = поверх

    UIDocument *document = nullptr;

    void SetDocument(UIDocument *doc) { document = doc; }

    Rect GetTargetRect(float screenW, float screenH) const
    {
        // Screen Space implemented only
        return Rect{0, 0, screenW, screenH};
    }

    // Лейаут + запись команд рисования в ctx. Вызывается из Scene::Render.
    void Render(UIContext &ctx, float screenW, float screenH)
    {
        if (!document || !IsEnabled())
            return;
        document->UpdateLayout(GetTargetRect(screenW, screenH));
        document->Render(ctx);
    }
};
