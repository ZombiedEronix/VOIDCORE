#pragma once
#include "Device/ICommandList.hpp"
#include "RenderPassEvent.hpp"

namespace VoidCore::Rendering
{
    class Renderer;
}

namespace VoidCore::Rendering
{
    class RenderPass
    {
    public:
        RenderPassEvent executionEvent = RenderPassEvent::AfterGeometry;
        virtual ~RenderPass() = default;
        virtual void OnResize(int width, int height) = 0;
        virtual void Execute(RHI::ICommandList *cmd, Renderer *renderer) = 0;
    };
}
