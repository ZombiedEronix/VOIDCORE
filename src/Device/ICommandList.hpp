#pragma once
#include "Device/RHITypes.hpp"

namespace VoidCore::RHI
{
    class IBuffer;
    class ITexture;
    class ISampler;
    class IPipeline;

    struct ColorAttachment
    {
        ITexture *texture = nullptr; // nullptr = back buffer окна
        LoadOp loadOp = LoadOp::Clear;
        float clearColor[4] = {0, 0, 0, 1};
    };

    struct DepthAttachment
    {
        ITexture *texture = nullptr; // nullptr = depth окна
        LoadOp loadOp = LoadOp::Clear;
        float clearDepth = 1.0f;
    };

    struct RenderPassDesc
    {
        ColorAttachment colors[kMaxColorTargets];
        uint32_t colorCount = 1;
        bool hasDepth = true;
        DepthAttachment depth;
        const char *debugName = nullptr;
    };

    // Все команды рисования записываются сюда. В OpenGL/D3D11 они исполняются
    // сразу, в Vulkan/Metal — реально записываются и уходят в очередь в EndFrame.
    // Движок не должен знать разницу.
    //
    // Контракты (одинаковы для всех бэкендов):
    //  * UpdateBuffer — ТОЛЬКО вне render pass.
    //  * Draw*         — ТОЛЬКО внутри render pass и после SetPipeline.
    //  * Состояние (пайплайн, буферы, слоты) НЕ переживает BeginRenderPass.
    //  * Dynamic-буфер можно обновлять раз за кадр; хочешь несколько кусков —
    //    пиши их подряд в один буфер и рисуй через firstInstance/offset.
    class ICommandList
    {
    public:
        virtual ~ICommandList() = default;

        // --- вне render pass ---
        virtual void UpdateBuffer(IBuffer *buffer, const void *data, uint64_t size, uint64_t offset = 0) = 0;

        // --- render pass ---
        virtual void BeginRenderPass(const RenderPassDesc &desc) = 0;
        virtual void EndRenderPass() = 0;

        // --- состояние ---
        virtual void SetPipeline(IPipeline *pipeline) = 0;
        virtual void SetViewport(const Viewport &viewport) = 0;
        virtual void SetScissor(const ScissorRect &scissor) = 0;

        // --- ресурсы ---
        virtual void SetVertexBuffer(uint32_t slot, IBuffer *buffer, uint64_t offset = 0) = 0;
        virtual void SetIndexBuffer(IBuffer *buffer, IndexType type, uint64_t offset = 0) = 0;
        // size = 0 → от offset до конца буфера
        virtual void SetUniformBuffer(uint32_t slot, IBuffer *buffer, uint64_t offset = 0, uint64_t size = 0) = 0;
        virtual void SetTexture(uint32_t slot, ITexture *texture, ISampler *sampler) = 0;

        // --- рисование ---
        virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 1,
                          uint32_t firstVertex = 0, uint32_t firstInstance = 0) = 0;
        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1,
                                 uint32_t firstIndex = 0, int32_t vertexOffset = 0,
                                 uint32_t firstInstance = 0) = 0;

        // --- отладка (RenderDoc / Nsight / Xcode) ---
        virtual void PushDebugGroup(const char *name) = 0;
        virtual void PopDebugGroup() = 0;
    };
}
