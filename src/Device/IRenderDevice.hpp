#pragma once
#include <cstdint>
#include <memory>

#include "Device/RHITypes.hpp"
#include "Device/IBuffer.hpp"
#include "Device/ITexture.hpp"
#include "Device/IShader.hpp"
#include "Device/IPipeline.hpp"
#include "Device/ICommandList.hpp"

struct SDL_Window; // SDL тянуть в публичный RHI-заголовок не нужно

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
        ShaderFormat shaderFormat = ShaderFormat::GLSL; // что принимает CreateShader
        uint32_t maxTextureSize = 0;
        uint32_t uniformBufferAlignment = 256; // для offset в SetUniformBuffer
        // Конвенция глубины одинакова везде: NDC z в [0, 1].
        // => в проекте нужен GLM_FORCE_DEPTH_ZERO_TO_ONE.
    };

    class IRenderDevice
    {
    public:
        virtual ~IRenderDevice() = default;

        virtual bool Initialize(SDL_Window *window, const DeviceDesc &desc) = 0;

        virtual GraphicsAPI GetAPI() const = 0;
        virtual const DeviceCaps &GetCaps() const = 0;

        // ---- Фабрика ресурсов (вместо RenderFactory с switch-ами) ----
        // При ошибке возвращают nullptr и пишут причину в лог.
        virtual Ref<IBuffer> CreateBuffer(const BufferDesc &desc) = 0;
        virtual Ref<ITexture> CreateTexture(const TextureDesc &desc) = 0;
        virtual Ref<ISampler> CreateSampler(const SamplerDesc &desc) = 0;
        virtual Ref<IShader> CreateShader(const ShaderDesc &desc) = 0;
        virtual Ref<IPipeline> CreatePipeline(const GraphicsPipelineDesc &desc) = 0;

        // ---- Кадр ----
        // BeginFrame: ждёт/берёт свободный кадр, возвращает основной command list.
        // EndFrame:   закрывает его, сабмитит и показывает (Present / SwapBuffers).
        virtual ICommandList *BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void GetBackBufferSize(uint32_t &width, uint32_t &height) const = 0;
        virtual void SetVerticalSync(bool enabled) = 0;

        // Дождаться GPU. ОБЯЗАТЕЛЬНО перед уничтожением ресурсов/устройства.
        virtual void WaitIdle() = 0;
    };
}
