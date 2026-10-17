#pragma once
#include <glad/gl.h>
#include "Device/ICommandList.hpp"
#include "OpenGLResources.hpp"

struct SDL_Window;

namespace VoidCore::RHI::OpenGL
{
    // В GL команды исполняются сразу, поэтому «запись» — это просто вызовы gl*.
    // Состояние (текущий пайплайн, vertex/index буферы) кэшируется здесь,
    // чтобы RHI-контракт был тем же, что у Vulkan/Metal: порядок
    // SetPipeline / SetVertexBuffer не важен, всё применяется перед Draw.
    class GLCommandList final : public ICommandList
    {
    public:
        explicit GLCommandList(SDL_Window *window);
        ~GLCommandList() override;

        void UpdateBuffer(IBuffer *buffer, const void *data, uint64_t size, uint64_t offset) override;

        void BeginRenderPass(const RenderPassDesc &desc) override;
        void EndRenderPass() override;

        void SetPipeline(IPipeline *pipeline) override;
        void SetViewport(const Viewport &viewport) override;
        void SetScissor(const ScissorRect &scissor) override;

        void SetVertexBuffer(uint32_t slot, IBuffer *buffer, uint64_t offset) override;
        void SetIndexBuffer(IBuffer *buffer, IndexType type, uint64_t offset) override;
        void SetUniformBuffer(uint32_t slot, IBuffer *buffer, uint64_t offset, uint64_t size) override;
        void SetTexture(uint32_t slot, ITexture *texture, ISampler *sampler) override;

        void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                         int32_t vertexOffset, uint32_t firstInstance) override;

        void PushDebugGroup(const char *name) override;
        void PopDebugGroup() override;

    private:
        void FlushVertexState();
        void ResetBindings();

        struct VertexBinding
        {
            const GLBuffer *buffer = nullptr;
            uint64_t offset = 0;
        };

        SDL_Window *m_window;
        GLuint m_fbo = 0; // переиспользуемый FBO для offscreen-проходов

        const GLPipeline *m_pipeline = nullptr;
        VertexBinding m_vertexBuffers[kMaxVertexBuffers];
        const GLBuffer *m_indexBuffer = nullptr;
        IndexType m_indexType = IndexType::UInt32;
        uint64_t m_indexOffset = 0;

        uint32_t m_vertexDirty = 0;
        bool m_indexDirty = false;
        bool m_inPass = false;
        bool m_passGroup = false;
    };
}
