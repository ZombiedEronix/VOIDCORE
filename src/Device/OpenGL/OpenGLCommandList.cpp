#include "OpenGLCommandList.hpp"
#include "OpenGLConvert.hpp"
#include "Core/Debug.h"

#include <SDL3/SDL.h>
#include <cstdint>

namespace VoidCore::RHI::OpenGL
{
    GLCommandList::GLCommandList(SDL_Window *window) : m_window(window)
    {
        glCreateFramebuffers(1, &m_fbo);
    }

    GLCommandList::~GLCommandList()
    {
        glDeleteFramebuffers(1, &m_fbo);
    }

    void GLCommandList::ResetBindings()
    {
        m_pipeline = nullptr;
        for (auto &vb : m_vertexBuffers)
            vb = {};
        m_indexBuffer = nullptr;
        m_vertexDirty = 0;
        m_indexDirty = false;
    }

    // ------------------------------------------------------------- upload ---
    void GLCommandList::UpdateBuffer(IBuffer *buffer, const void *data, uint64_t size, uint64_t offset)
    {
        if (m_inPass)
            Debug::LogWarning("[RHI] UpdateBuffer inside render pass — запрещено контрактом (Vulkan/Metal так не умеют)");

        auto *glBuf = static_cast<GLBuffer *>(buffer);
        if (!glBuf || !data || size == 0)
            return;
        if (glBuf->GetDesc().memory != MemoryType::Dynamic)
        {
            Debug::LogError("[RHI] UpdateBuffer: buffer is not MemoryType::Dynamic");
            return;
        }
        glNamedBufferSubData(glBuf->Handle(), static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    }

    // -------------------------------------------------------- render pass ---
    void GLCommandList::BeginRenderPass(const RenderPassDesc &desc)
    {
        ResetBindings();
        m_inPass = true;
        m_passGroup = desc.debugName != nullptr;
        if (m_passGroup)
            PushDebugGroup(desc.debugName);

        const bool offscreen = desc.colors[0].texture != nullptr ||
                               (desc.hasDepth && desc.depth.texture != nullptr);

        GLuint fbo = 0;
        int width = 0, height = 0;

        if (!offscreen)
        {
            SDL_GetWindowSizeInPixels(m_window, &width, &height);
        }
        else
        {
            fbo = m_fbo;
            GLenum drawBuffers[kMaxColorTargets];
            for (uint32_t i = 0; i < desc.colorCount; ++i)
            {
                auto *tex = static_cast<GLTexture *>(desc.colors[i].texture);
                glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0 + i, tex ? tex->Handle() : 0, 0);
                drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
                if (tex && i == 0)
                {
                    width = static_cast<int>(tex->GetDesc().width);
                    height = static_cast<int>(tex->GetDesc().height);
                }
            }
            glNamedFramebufferDrawBuffers(fbo, static_cast<GLsizei>(desc.colorCount), drawBuffers);

            auto *depthTex = static_cast<GLTexture *>(desc.depth.texture);
            const GLenum depthAttachment =
                (depthTex && depthTex->GetDesc().format == Format::D24_UNORM_S8_UINT)
                    ? GL_DEPTH_STENCIL_ATTACHMENT
                    : GL_DEPTH_ATTACHMENT;
            glNamedFramebufferTexture(fbo, depthAttachment, depthTex ? depthTex->Handle() : 0, 0);
            if (width == 0 && depthTex)
            {
                width = static_cast<int>(depthTex->GetDesc().width);
                height = static_cast<int>(depthTex->GetDesc().height);
            }
        }

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

        // Сбрасываем всё, что может помешать clear'у, и ставим полный viewport/scissor.
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 0, width, height);
        glViewport(0, 0, width, height);
        glDepthRange(0.0, 1.0);
        glDepthMask(GL_TRUE);
        for (uint32_t i = 0; i < desc.colorCount; ++i)
            glColorMaski(i, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        for (uint32_t i = 0; i < desc.colorCount; ++i)
        {
            if (desc.colors[i].loadOp == LoadOp::Clear)
                glClearNamedFramebufferfv(fbo, GL_COLOR, static_cast<GLint>(i), desc.colors[i].clearColor);
        }
        if (desc.hasDepth && desc.depth.loadOp == LoadOp::Clear)
            glClearNamedFramebufferfv(fbo, GL_DEPTH, 0, &desc.depth.clearDepth);
    }

    void GLCommandList::EndRenderPass()
    {
        if (!m_inPass)
            return;
        m_inPass = false;
        if (m_passGroup)
            PopDebugGroup();
        m_passGroup = false;
    }

    // -------------------------------------------------------------- state ---
    void GLCommandList::SetPipeline(IPipeline *pipeline)
    {
        auto *p = static_cast<GLPipeline *>(pipeline);
        if (p == m_pipeline)
            return;
        m_pipeline = p;
        if (!p)
            return;

        p->Apply();
        // VAO у каждого пайплайна свой → перепривязать буферы лениво, на Draw.
        m_vertexDirty = (1u << kMaxVertexBuffers) - 1u;
        m_indexDirty = true;
    }

    void GLCommandList::SetViewport(const Viewport &vp)
    {
        glViewportIndexedf(0, vp.x, vp.y, vp.width, vp.height);
        glDepthRangeIndexed(0, vp.minDepth, vp.maxDepth);
    }

    void GLCommandList::SetScissor(const ScissorRect &r)
    {
        glScissor(r.x, r.y, static_cast<GLsizei>(r.width), static_cast<GLsizei>(r.height));
    }

    // ---------------------------------------------------------- resources ---
    void GLCommandList::SetVertexBuffer(uint32_t slot, IBuffer *buffer, uint64_t offset)
    {
        if (slot >= kMaxVertexBuffers)
            return;
        m_vertexBuffers[slot] = {static_cast<GLBuffer *>(buffer), offset};
        m_vertexDirty |= (1u << slot);
    }

    void GLCommandList::SetIndexBuffer(IBuffer *buffer, IndexType type, uint64_t offset)
    {
        m_indexBuffer = static_cast<GLBuffer *>(buffer);
        m_indexType = type;
        m_indexOffset = offset;
        m_indexDirty = true;
    }

    void GLCommandList::SetUniformBuffer(uint32_t slot, IBuffer *buffer, uint64_t offset, uint64_t size)
    {
        if (slot >= kMaxUniformBuffers)
            return;
        auto *b = static_cast<GLBuffer *>(buffer);
        if (!b)
        {
            glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
            return;
        }
        if (size == 0)
            size = b->GetDesc().size - offset;
        glBindBufferRange(GL_UNIFORM_BUFFER, slot, b->Handle(),
                          static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size));
    }

    void GLCommandList::SetTexture(uint32_t slot, ITexture *texture, ISampler *sampler)
    {
        if (slot >= kMaxTextureSlots)
            return;
        auto *t = static_cast<GLTexture *>(texture);
        auto *s = static_cast<GLSampler *>(sampler);
        glBindTextureUnit(slot, t ? t->Handle() : 0);
        glBindSampler(slot, s ? s->Handle() : 0);
    }

    void GLCommandList::FlushVertexState()
    {
        if (!m_pipeline)
            return;
        const GLuint vao = m_pipeline->VAO();

        for (uint32_t slot = 0; slot < kMaxVertexBuffers; ++slot)
        {
            if (!(m_vertexDirty & (1u << slot)))
                continue;
            const VertexBinding &vb = m_vertexBuffers[slot];
            glVertexArrayVertexBuffer(vao, slot, vb.buffer ? vb.buffer->Handle() : 0,
                                      static_cast<GLintptr>(vb.offset),
                                      static_cast<GLsizei>(m_pipeline->Stride(slot)));
        }
        m_vertexDirty = 0;

        if (m_indexDirty)
        {
            glVertexArrayElementBuffer(vao, m_indexBuffer ? m_indexBuffer->Handle() : 0);
            m_indexDirty = false;
        }
    }

    // ------------------------------------------------------------- draws ---
    void GLCommandList::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
    {
        if (!m_pipeline)
            return;
        FlushVertexState();
        glDrawArraysInstancedBaseInstance(m_pipeline->Topology(), static_cast<GLint>(firstVertex),
                                          static_cast<GLsizei>(vertexCount),
                                          static_cast<GLsizei>(instanceCount), firstInstance);
    }

    void GLCommandList::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                                    int32_t vertexOffset, uint32_t firstInstance)
    {
        if (!m_pipeline || !m_indexBuffer)
            return;
        FlushVertexState();
        const uintptr_t byteOffset = static_cast<uintptr_t>(m_indexOffset) +
                                     static_cast<uintptr_t>(firstIndex) * IndexSize(m_indexType);
        glDrawElementsInstancedBaseVertexBaseInstance(
            m_pipeline->Topology(), static_cast<GLsizei>(indexCount), ToGL(m_indexType),
            reinterpret_cast<const void *>(byteOffset), static_cast<GLsizei>(instanceCount),
            vertexOffset, firstInstance);
    }

    // -------------------------------------------------------------- debug ---
    void GLCommandList::PushDebugGroup(const char *name)
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name);
    }

    void GLCommandList::PopDebugGroup()
    {
        glPopDebugGroup();
    }
}
