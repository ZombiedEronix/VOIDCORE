#pragma once
#include "Device/ICommandList.hpp"
#include "RenderPassEvent.hpp"

class Renderer;

namespace VoidCore::Rendering
{
    class RenderPass
    {
    public:
        RenderPassEvent executionEvent = RenderPassEvent::AfterGeometry;
        virtual ~RenderPass() = default;
        virtual void Execute(RHI::ICommandList *cmd, Renderer *renderer) = 0;
        virtual void OnResize(int width, int height) {}
    };
}
