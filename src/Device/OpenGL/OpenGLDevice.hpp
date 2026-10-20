#pragma once
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <memory>
#include <string>

#include "Device/IRenderDevice.hpp"
#include "OpenGLCommandList.hpp"

namespace VoidCore::RHI::OpenGL
{
    // Выставить GL-атрибуты контекста. Вызывать до SDL_CreateWindow.
    void ConfigureWindowAttributes(bool debug);

    class OpenGLDevice final : public IRenderDevice
    {
    public:
        OpenGLDevice() = default;
        ~OpenGLDevice() override;

        bool Initialize(SDL_Window *window, const DeviceDesc &desc) override;

        GraphicsAPI GetAPI() const override { return GraphicsAPI::OpenGL; }
        const DeviceCaps &GetCaps() const override { return m_caps; }

        Ref<IBuffer> CreateBuffer(const BufferDesc &desc) override;
        Ref<ITexture> CreateTexture(const TextureDesc &desc) override;
        Ref<ISampler> CreateSampler(const SamplerDesc &desc) override;
        Ref<IShader> CreateShader(const ShaderDesc &desc) override;
        Ref<IPipeline> CreatePipeline(const GraphicsPipelineDesc &desc) override;

        ICommandList *BeginFrame() override;
        void EndFrame() override;

        void GetBackBufferSize(uint32_t &width, uint32_t &height) const override;
        void SetVerticalSync(bool enabled) override;
        void WaitIdle() override;

    private:
        SDL_Window *m_window = nullptr;
        SDL_GLContext m_context = nullptr;
        DeviceCaps m_caps;
        std::string m_deviceName;
        std::unique_ptr<GLCommandList> m_cmd;
    };
}
