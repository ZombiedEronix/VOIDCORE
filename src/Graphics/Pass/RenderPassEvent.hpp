#pragma once

namespace VoidCore::Rendering
{
    enum class RenderPassEvent : int
    {
        BeforeGeometry = 100,
        AfterGeometry = 200,
        AfterLighting = 300,
        BeforePostProcessing = 400,
        AfterPostProcessing = 500,
        BeforeUI = 600,
        AfterUI = 700,
    };
}
