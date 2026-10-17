#pragma once
#include <glad/gl.h>
#include <vector>

#include "Device/IBuffer.hpp"
#include "Device/ITexture.hpp"
#include "Device/IShader.hpp"
#include "Device/IPipeline.hpp"

namespace VoidCore::RHI::OpenGL
{
    class GLBuffer final : public IBuffer
    {
    public:
        explicit GLBuffer(const BufferDesc &desc);
        ~GLBuffer() override;

        const BufferDesc &GetDesc() const override { return m_desc; }
        uint64_t GetNativeHandle() const override { return m_id; }
        GLuint Handle() const { return m_id; }

    private:
        BufferDesc m_desc;
        GLuint m_id = 0;
    };

    class GLTexture final : public ITexture
    {
    public:
        explicit GLTexture(const TextureDesc &desc);
        ~GLTexture() override;

        const TextureDesc &GetDesc() const override { return m_desc; }
        uint64_t GetNativeHandle() const override { return m_id; }
        GLuint Handle() const { return m_id; }

    private:
        TextureDesc m_desc;
        GLuint m_id = 0;
    };

    class GLSampler final : public ISampler
    {
    public:
        explicit GLSampler(const SamplerDesc &desc);
        ~GLSampler() override;

        const SamplerDesc &GetDesc() const override { return m_desc; }
        uint64_t GetNativeHandle() const override { return m_id; }
        GLuint Handle() const { return m_id; }

    private:
        SamplerDesc m_desc;
        GLuint m_id = 0;
    };

    class GLShader final : public IShader
    {
    public:
        GLShader(const ShaderDesc &desc);
        ~GLShader() override;

        ShaderStage GetStage() const override { return m_stage; }
        uint64_t GetNativeHandle() const override { return m_id; }
        GLuint Handle() const { return m_id; }
        bool IsValid() const { return m_valid; }

    private:
        ShaderStage m_stage;
        GLuint m_id = 0;
        bool m_valid = false;
    };

    class GLPipeline final : public IPipeline
    {
    public:
        explicit GLPipeline(const GraphicsPipelineDesc &desc);
        ~GLPipeline() override;

        uint64_t GetNativeHandle() const override { return m_program; }
        bool IsValid() const { return m_valid; }

        // Применить фиксированное состояние (вызывается из command list).
        void Apply() const;

        GLuint Program() const { return m_program; }
        GLuint VAO() const { return m_vao; }
        GLenum Topology() const { return m_topology; }
        uint32_t Stride(uint32_t slot) const { return slot < kMaxVertexBuffers ? m_strides[slot] : 0; }

    private:
        GLuint m_program = 0;
        GLuint m_vao = 0;
        GLenum m_topology = 0;
        uint32_t m_strides[kMaxVertexBuffers] = {};
        bool m_valid = false;

        // копия состояния — исходные указатели из desc нам не принадлежат
        RasterizerState m_raster;
        DepthState m_depth;
        BlendState m_blend[kMaxColorTargets];
        uint32_t m_colorTargetCount = 1;
    };
}
