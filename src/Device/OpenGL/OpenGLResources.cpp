#include "OpenGLResources.hpp"
#include "OpenGLConvert.hpp"
#include "Core/Debug.h"

#include <algorithm>
#include <string>

namespace VoidCore::RHI::OpenGL
{
    static void Label(GLenum type, GLuint id, const char *name)
    {
        if (name && *name)
            glObjectLabel(type, id, -1, name);
    }

    // ------------------------------------------------------------- Buffer ---
    GLBuffer::GLBuffer(const BufferDesc &desc) : m_desc(desc)
    {
        m_desc.initialData = nullptr; // указатель принадлежит вызывающему
        glCreateBuffers(1, &m_id);

        // Immutable storage: размер фиксирован. Dynamic = можно писать через SubData.
        GLbitfield flags = (desc.memory == MemoryType::Dynamic) ? GL_DYNAMIC_STORAGE_BIT : 0;
        glNamedBufferStorage(m_id, static_cast<GLsizeiptr>(desc.size), desc.initialData, flags);
        Label(GL_BUFFER, m_id, desc.debugName);
    }

    GLBuffer::~GLBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    // ------------------------------------------------------------ Texture ---
    GLTexture::GLTexture(const TextureDesc &desc) : m_desc(desc)
    {
        m_desc.initialData = nullptr;

        uint32_t levels = desc.mipLevels;
        if (levels == 0)
        {
            levels = 1;
            for (uint32_t s = std::max(desc.width, desc.height); s > 1; s >>= 1)
                ++levels;
        }
        m_desc.mipLevels = levels;

        const GLTextureFormat fmt = ToGL(desc.format);
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureStorage2D(m_id, static_cast<GLsizei>(levels), fmt.internalFormat,
                           static_cast<GLsizei>(desc.width), static_cast<GLsizei>(desc.height));

        if (desc.initialData)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // ширина*bpp не обязана быть кратна 4
            glTextureSubImage2D(m_id, 0, 0, 0, static_cast<GLsizei>(desc.width),
                                static_cast<GLsizei>(desc.height), fmt.format, fmt.type, desc.initialData);
            if (levels > 1)
                glGenerateTextureMipmap(m_id);
        }
        Label(GL_TEXTURE, m_id, desc.debugName);
    }

    GLTexture::~GLTexture()
    {
        glDeleteTextures(1, &m_id);
    }

    // ------------------------------------------------------------ Sampler ---
    GLSampler::GLSampler(const SamplerDesc &desc) : m_desc(desc)
    {
        glCreateSamplers(1, &m_id);
        glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, ToGLMinFilter(desc.minFilter, desc.mipFilter));
        glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, desc.magFilter == Filter::Nearest ? GL_NEAREST : GL_LINEAR);
        glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, static_cast<GLint>(ToGL(desc.addressU)));
        glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, static_cast<GLint>(ToGL(desc.addressV)));
#if defined(GL_TEXTURE_MAX_ANISOTROPY)
        if (desc.maxAnisotropy > 1.0f)
            glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY, desc.maxAnisotropy);
#endif
        Label(GL_SAMPLER, m_id, desc.debugName);
    }

    GLSampler::~GLSampler()
    {
        glDeleteSamplers(1, &m_id);
    }

    // ------------------------------------------------------------- Shader ---
    GLShader::GLShader(const ShaderDesc &desc) : m_stage(desc.stage)
    {
        m_id = glCreateShader(desc.stage == ShaderStage::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

        const std::string source(static_cast<const char *>(desc.code), desc.codeSize);
        const char *src = source.c_str();
        glShaderSource(m_id, 1, &src, nullptr);
        glCompileShader(m_id);

        GLint ok = 0;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &ok);
        m_valid = (ok != 0);
        if (!m_valid)
        {
            char log[2048];
            glGetShaderInfoLog(m_id, sizeof(log), nullptr, log);
            Debug::LogError("[GL] ", desc.debugName ? desc.debugName : "shader", " compile error:\n", log);
        }
        Label(GL_SHADER, m_id, desc.debugName);
    }

    GLShader::~GLShader()
    {
        glDeleteShader(m_id);
    }

    // ----------------------------------------------------------- Pipeline ---
    GLPipeline::GLPipeline(const GraphicsPipelineDesc &desc)
        : m_topology(ToGL(desc.topology)),
          m_raster(desc.rasterizer),
          m_depth(desc.depth),
          m_colorTargetCount(desc.colorTargetCount)
    {
        for (uint32_t i = 0; i < kMaxColorTargets; ++i)
            m_blend[i] = desc.blend[i];

        auto *vs = static_cast<GLShader *>(desc.vertexShader);
        auto *fs = static_cast<GLShader *>(desc.fragmentShader);
        if (!vs || !fs || !vs->IsValid() || !fs->IsValid())
        {
            Debug::LogError("[GL] CreatePipeline: invalid shaders");
            return;
        }

        // --- program ---
        m_program = glCreateProgram();
        glAttachShader(m_program, vs->Handle());
        glAttachShader(m_program, fs->Handle());
        glLinkProgram(m_program);
        glDetachShader(m_program, vs->Handle());
        glDetachShader(m_program, fs->Handle());

        GLint ok = 0;
        glGetProgramiv(m_program, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            char log[2048];
            glGetProgramInfoLog(m_program, sizeof(log), nullptr, log);
            Debug::LogError("[GL] Program link error:\n", log);
            return;
        }

        // --- vertex input (VAO принадлежит пайплайну) ---
        glCreateVertexArrays(1, &m_vao);
        for (const VertexAttribute &a : desc.vertexAttributes)
        {
            const GLVertexFormat f = ToGLVertex(a.format);
            glEnableVertexArrayAttrib(m_vao, a.location);
            if (f.isInteger)
                glVertexArrayAttribIFormat(m_vao, a.location, f.components, f.type, a.offset);
            else
                glVertexArrayAttribFormat(m_vao, a.location, f.components, f.type, f.normalized, a.offset);
            glVertexArrayAttribBinding(m_vao, a.location, a.bufferSlot);
        }
        for (uint32_t slot = 0; slot < desc.vertexBuffers.size() && slot < kMaxVertexBuffers; ++slot)
        {
            m_strides[slot] = desc.vertexBuffers[slot].stride;
            glVertexArrayBindingDivisor(m_vao, slot,
                                        desc.vertexBuffers[slot].inputRate == VertexInputRate::PerInstance ? 1 : 0);
        }

        Label(GL_PROGRAM, m_program, desc.debugName);
        m_valid = true;
    }

    GLPipeline::~GLPipeline()
    {
        if (m_program)
            glDeleteProgram(m_program);
        if (m_vao)
            glDeleteVertexArrays(1, &m_vao);
    }

    void GLPipeline::Apply() const
    {
        glUseProgram(m_program);
        glBindVertexArray(m_vao);

        // rasterizer
        if (m_raster.cullMode == CullMode::None)
            glDisable(GL_CULL_FACE);
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(m_raster.cullMode == CullMode::Back ? GL_BACK : GL_FRONT);
        }
        glFrontFace(m_raster.frontFace == FrontFace::CounterClockwise ? GL_CCW : GL_CW);

        // depth
        if (m_depth.testEnable)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        glDepthFunc(ToGL(m_depth.compareOp));
        glDepthMask(m_depth.writeEnable ? GL_TRUE : GL_FALSE);

        // blend (по целям)
        for (uint32_t i = 0; i < m_colorTargetCount; ++i)
        {
            const BlendState &b = m_blend[i];
            if (b.enable)
            {
                glEnablei(GL_BLEND, i);
                glBlendFuncSeparatei(i, ToGL(b.srcColor), ToGL(b.dstColor), ToGL(b.srcAlpha), ToGL(b.dstAlpha));
                glBlendEquationSeparatei(i, ToGL(b.colorOp), ToGL(b.alphaOp));
            }
            else
                glDisablei(GL_BLEND, i);

            const uint8_t m = static_cast<uint8_t>(b.writeMask);
            glColorMaski(i, (m & 1) != 0, (m & 2) != 0, (m & 4) != 0, (m & 8) != 0);
        }
    }
}
