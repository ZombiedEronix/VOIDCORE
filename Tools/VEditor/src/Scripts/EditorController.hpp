#pragma once
#include "Core/GarbageCollector/Ref.hpp"
#include "Entity/Components/System/UI/UICanvas.hpp"

class EditorController : public Behaviour
{
public:
    Ref<GameObject> cam;
    void OnEnable() override
    {
        Debug::Log("Editor Initialized!");
    }

    void Update() override
    {
        if (cam != nullptr)
        {
            Gizmo::DrawGrid(cam->transform->GetPosition(), 50, 1.0f);
        }
    }
};