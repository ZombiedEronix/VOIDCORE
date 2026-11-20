#pragma once
#include <cstdint>
#include <memory>

#include "Device/RHITypes.hpp"
#include "Device/IBuffer.hpp"
#include "Device/ITexture.hpp"
#include "Device/IShader.hpp"
#include "Device/IPipeline.hpp"
#include "Device/ICommandList.hpp"

struct SDL_Window; // There is no need to drag SDL into the public RHI header

namespace VoidCore::RHI
{
    struct DeviceDesc
    {
        bool enableValidation = false; // debug layer / KHR_debug / Metal validation
        bool vsync = true;
    };

    struct DeviceCaps
    {
        GraphicsAPI api = GraphicsAPI::OpenGL;
        const char *deviceName = "";
        ShaderFormat shaderFormat = ShaderFormat::GLSL; // what does CreateShader accept?
        uint32_t maxTextureSize = 0;
        uint32_t uniformBufferAlignment = 256; // for offset in SetUniformBuffer
        // The depth convention is the same everywhere: NDC z at [0, 1].
        // => the project needs GLM_FORCE_DEPTH_ZERO_TO_ONE.
    };

    class IRenderDevice
    {
    public:
        virtual ~IRenderDevice() = default;

        virtual bool Initialize(SDL_Window *window, const DeviceDesc &desc) = 0;

        virtual GraphicsAPI GetAPI() const = 0;
        virtual const DeviceCaps &GetCaps() const = 0;

        // ---- Resource factory (instead of RenderFactory with switches) ----
        // If there is an error, they return nullptr and write the reason to the log.
        virtual Ref<IBuffer> CreateBuffer(const BufferDesc &desc) = 0;
        virtual Ref<ITexture> CreateTexture(const TextureDesc &desc) = 0;
        virtual Ref<ISampler> CreateSampler(const SamplerDesc &desc) = 0;
        virtual Ref<IShader> CreateShader(const ShaderDesc &desc) = 0;
        virtual Ref<IPipeline> CreatePipeline(const GraphicsPipelineDesc &desc) = 0;

        // ---- Cadre ----
        // BeginFrame: waits/takes a free frame, returns the main command list.
        // EndFrame: closes it, submits and shows (Present / SwapBuffers).
        virtual ICommandList *BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void GetBackBufferSize(uint32_t &width, uint32_t &height) const = 0;
        virtual void SetVerticalSync(bool enabled) = 0;

        // Wait for the GPU. MANDATORY before destroying resources/device.
        virtual void WaitIdle() = 0;
    };
}
