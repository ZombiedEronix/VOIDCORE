#include "OpenGLDevice.hpp"
#include "OpenGLResources.hpp"
#include "OpenGLConvert.hpp"
#include "Core/Debug.h"

namespace VoidCore::RHI::OpenGL
{
    void ConfigureWindowAttributes(bool debug)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        if (debug)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    }

    static void GLAPIENTRY DebugCallback(GLenum, GLenum type, GLuint id, GLenum severity,
                                         GLsizei, const GLchar *message, const void *)
    {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
            return;
        if (type == GL_DEBUG_TYPE_ERROR)
            Debug::LogError("[GL ", id, "] ", message);
        else
            Debug::LogWarning("[GL ", id, "] ", message);
    }

    bool OpenGLDevice::Initialize(SDL_Window *window, const DeviceDesc &desc)
    {
        m_window = window;

        m_context = SDL_GL_CreateContext(window);
        if (!m_context)
        {
            Debug::LogError("Failed to create OpenGL context: ", SDL_GetError());
            return false;
        }
        SDL_GL_MakeCurrent(window, m_context);

        const int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
        if (version == 0)
        {
            Debug::LogError("GLAD failed to load OpenGL");
            return false;
        }
        const int major = GLAD_VERSION_MAJOR(version);
        const int minor = GLAD_VERSION_MINOR(version);
        if (major < 4 || (major == 4 && minor < 5))
        {
            Debug::LogError("OpenGL 4.5 required, got ", major, ".", minor,
                            " (regenerate glad: gl=4.5 core)");
            return false;
        }

        // Единая конвенция глубины для всех API: NDC z ∈ [0,1].
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

        if (desc.enableValidation)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(DebugCallback, nullptr);
        }

        m_deviceName = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
        GLint maxTex = 0, uboAlign = 256;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTex);
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uboAlign);

        m_caps.api = GraphicsAPI::OpenGL;
        m_caps.deviceName = m_deviceName.c_str();
        m_caps.shaderFormat = ShaderFormat::GLSL;
        m_caps.maxTextureSize = static_cast<uint32_t>(maxTex);
        m_caps.uniformBufferAlignment = static_cast<uint32_t>(uboAlign);

        m_cmd = std::make_unique<GLCommandList>(window);
        SetVerticalSync(desc.vsync);

        Debug::Log("OpenGL Device Initialized: ", reinterpret_cast<const char *>(glGetString(GL_VERSION)), " | ", m_deviceName);
        return true;
    }

    OpenGLDevice::~OpenGLDevice()
    {
        if (m_context)
        {
            WaitIdle();
            m_cmd.reset(); // FBO нужно удалить, пока жив контекст
            SDL_GL_DestroyContext(m_context);
        }
    }

    // ------------------------------------------------------------ ресурсы ---
    Ref<IBuffer> OpenGLDevice::CreateBuffer(const BufferDesc &desc)
    {
        if (desc.size == 0)
        {
            Debug::LogError("[RHI] CreateBuffer: size == 0");
            return nullptr;
        }
        if (desc.memory == MemoryType::Static && !desc.initialData)
        {
            Debug::LogError("[RHI] CreateBuffer: Static buffer needs initialData");
            return nullptr;
        }
        return std::make_shared<GLBuffer>(desc);
    }

    Ref<ITexture> OpenGLDevice::CreateTexture(const TextureDesc &desc)
    {
        if (ToGL(desc.format).internalFormat == 0)
        {
            Debug::LogError("[RHI] CreateTexture: unsupported format");
            return nullptr;
        }
        return std::make_shared<GLTexture>(desc);
    }

    Ref<ISampler> OpenGLDevice::CreateSampler(const SamplerDesc &desc)
    {
        return std::make_shared<GLSampler>(desc);
    }

    Ref<IShader> OpenGLDevice::CreateShader(const ShaderDesc &desc)
    {
        if (desc.format != ShaderFormat::GLSL || !desc.code || desc.codeSize == 0)
        {
            Debug::LogError("[RHI] OpenGL backend accepts only ShaderFormat::GLSL source");
            return nullptr;
        }
        auto shader = std::make_shared<GLShader>(desc);
        return shader->IsValid() ? shader : nullptr;
    }

    Ref<IPipeline> OpenGLDevice::CreatePipeline(const GraphicsPipelineDesc &desc)
    {
        auto pipeline = std::make_shared<GLPipeline>(desc);
        return pipeline->IsValid() ? pipeline : nullptr;
    }

    // --------------------------------------------------------------- кадр ---
    ICommandList *OpenGLDevice::BeginFrame()
    {
        return m_cmd.get();
    }

    void OpenGLDevice::EndFrame()
    {
        SDL_GL_SwapWindow(m_window);
    }

    void OpenGLDevice::GetBackBufferSize(uint32_t &width, uint32_t &height) const
    {
        int w = 0, h = 0;
        SDL_GetWindowSizeInPixels(m_window, &w, &h);
        width = static_cast<uint32_t>(w);
        height = static_cast<uint32_t>(h);
    }

    void OpenGLDevice::SetVerticalSync(bool enabled)
    {
        SDL_GL_SetSwapInterval(enabled ? 1 : 0);
    }

    void OpenGLDevice::WaitIdle()
    {
        glFinish();
    }
}
