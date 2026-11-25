#pragma once
#include "Device/RHI.hpp"
#include "Graphics/Pass/RenderPass.hpp"
#include "Graphics/Renderer.hpp"
#include "GizmoVertex.hpp"

namespace VEditor::Rendering
{
    class GizmoPass : public VoidCore::Rendering::RenderPass
    {
    private:
        VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> linePipeline;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> vertexBuffer;
        size_t vertexCapacity = 0;

        void EnsureBuffer(size_t count);
        void CreatePipeline();

    public:
        GizmoPass();
        void OnResize(int width, int height) override {}
        void Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer) override;
    };
}
