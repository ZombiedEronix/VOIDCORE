#include "GizmoPass.hpp"
#include "Assets/ResourceManager.h"
#include "Device/IBuffer.hpp"
#include <algorithm>
#include <cstddef>
#include "Gizmo.hpp"
namespace VEditor::Rendering
{
    using namespace VoidCore::RHI;

    GizmoPass::GizmoPass()
    {
        executionEvent = VoidCore::Rendering::RenderPassEvent::AfterPostProcessing;
        CreatePipeline();
        EnsureBuffer(1024);
    }

    void GizmoPass::EnsureBuffer(size_t count)
    {
        if (!vertexBuffer || count > vertexCapacity)
        {
            vertexCapacity = std::max<size_t>(count * 2, 1024);
            BufferDesc desc;
            desc.size = vertexCapacity * sizeof(GizmoVertex);
            desc.usage = BufferUsage::Vertex;
            desc.memory = MemoryType::Dynamic;
            desc.debugName = "GizmoVertexBuffer";
            vertexBuffer = GetDevice().CreateBuffer(desc);
        }
    }

    void GizmoPass::CreatePipeline()
    {
        auto shader = ResourceManager::GetShader("Assets/shaders/Editor/gizmo");
        if (!shader || !shader->IsValid())
            return;

        GraphicsPipelineDesc desc;
        desc.vertexShader = shader->GetVS();
        desc.fragmentShader = shader->GetFS();

        desc.vertexBuffers = {{sizeof(GizmoVertex), VertexInputRate::PerVertex}};
        desc.vertexAttributes = {
            {0, Format::RGB32_FLOAT, (uint32_t)offsetof(GizmoVertex, pos), 0},
            {1, Format::RGBA32_FLOAT, (uint32_t)offsetof(GizmoVertex, color), 0},
        };

        desc.topology = PrimitiveTopology::LineList;
        desc.rasterizer.cullMode = CullMode::None;
        desc.depth.testEnable = false;
        desc.depth.writeEnable = false;

        desc.blend[0].enable = true;
        desc.blend[0].srcColor = BlendFactor::SrcAlpha;
        desc.blend[0].dstColor = BlendFactor::OneMinusSrcAlpha;
        desc.blend[0].colorOp = BlendOp::Add;

        desc.colorTargetCount = 1;
        desc.colorFormats[0] = Format::RGBA8_UNORM;
        desc.depthFormat = Format::Unknown;
        desc.debugName = "GizmoLinePipeline";

        linePipeline = GetDevice().CreatePipeline(desc);
    }

    void GizmoPass::Execute(ICommandList *cmd, Renderer *renderer)
    {
        const auto &verts = Gizmo::GetVertices();
        if (!cmd || !linePipeline || verts.empty())
        {
            Gizmo::Clear();
            return;
        }

        EnsureBuffer(verts.size());
        cmd->UpdateBuffer(vertexBuffer.get(), verts.data(), verts.size() * sizeof(GizmoVertex));

        RenderPassDesc pass;
        pass.debugName = "Gizmo Pass";
        pass.colorCount = 1;
        pass.colors[0].texture = renderer->GetRenderTargetTexture("GBuffer_RenderResult");
        pass.colors[0].loadOp = LoadOp::Load;
        pass.hasDepth = false;

        cmd->BeginRenderPass(pass);
        cmd->SetPipeline(linePipeline.get());
        cmd->SetUniformBuffer(0, renderer->GetCameraBuffer());
        cmd->SetVertexBuffer(0, vertexBuffer.get());
        cmd->Draw(static_cast<uint32_t>(verts.size()), 1, 0, 0);
        cmd->EndRenderPass();

        Gizmo::Clear();
    }
}
