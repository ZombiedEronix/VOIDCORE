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
        ITexture *texture = nullptr; // nullptr = window back buffer
        LoadOp loadOp = LoadOp::Clear;
        float clearColor[4] = {0, 0, 0, 1};
    };

    struct DepthAttachment
    {
        ITexture *texture = nullptr; // nullptr = window depth
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

    // All drawing commands are written here. In OpenGL/D3D11 they are executed
    // immediately, in Vulkan/Metal - they are actually recorded and queued in EndFrame.
    // The engine shouldn't know the difference.
    //
    // Contracts (same for all backends):
    // * UpdateBuffer - ONLY outside the render pass.
    // * Draw * - ONLY inside the render pass and after SetPipeline.
    // * State (pipeline, buffers, slots) does NOT survive BeginRenderPass.
    // * Dynamic buffer can be updated once per frame; if you want a few pieces -
    // write them in a row into one buffer and draw via firstInstance/offset.
    class ICommandList
    {
    public:
        virtual ~ICommandList() = default;

        // --- out render pass ---
        virtual void UpdateBuffer(IBuffer *buffer, const void *data, uint64_t size, uint64_t offset = 0) = 0;

        // --- render pass ---
        virtual void BeginRenderPass(const RenderPassDesc &desc) = 0;
        virtual void EndRenderPass() = 0;

        // --- state ---
        virtual void SetPipeline(IPipeline *pipeline) = 0;
        virtual void SetViewport(const Viewport &viewport) = 0;
        virtual void SetScissor(const ScissorRect &scissor) = 0;

        // --- resources ---
        virtual void SetVertexBuffer(uint32_t slot, IBuffer *buffer, uint64_t offset = 0) = 0;
        virtual void SetIndexBuffer(IBuffer *buffer, IndexType type, uint64_t offset = 0) = 0;
        // size = 0 → from offset to end of buffer
        virtual void SetUniformBuffer(uint32_t slot, IBuffer *buffer, uint64_t offset = 0, uint64_t size = 0) = 0;
        virtual void SetTexture(uint32_t slot, ITexture *texture, ISampler *sampler) = 0;

        // --- drawing ---
        virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 1,
                          uint32_t firstVertex = 0, uint32_t firstInstance = 0) = 0;
        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1,
                                 uint32_t firstIndex = 0, int32_t vertexOffset = 0,
                                 uint32_t firstInstance = 0) = 0;

        // --- debugging (RenderDoc/Nsight/Xcode) ---
        virtual void PushDebugGroup(const char *name) = 0;
        virtual void PopDebugGroup() = 0;
    };
}
