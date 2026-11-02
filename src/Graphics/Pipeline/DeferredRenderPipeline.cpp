#include "DefferedRenderPipeline.hpp"
#include "Core/Debug.h"

namespace VoidCore::Rendering::Deferred
{
    using namespace VoidCore::RHI;

    void DeferredRenderPipeline::OnResize(int width, int height)
    {

    }

    void DeferredRenderPipeline::Execute(Renderer *renderer)
    {
        auto cmd = renderer->GetCommandList();
        if (!cmd)
            return;
    }
}