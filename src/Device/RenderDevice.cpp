#include "Device/RenderDevice.hpp"
#include "Core/Debug.h"
#include <SDL3/SDL.h>

// Какие бэкенды собирать. Задаются в CMake: target_compile_definitions(... VC_RHI_VULKAN=1)
#ifndef VC_RHI_OPENGL
#define VC_RHI_OPENGL 1
#endif
#ifndef VC_RHI_VULKAN
#define VC_RHI_VULKAN 0
#endif
#ifndef VC_RHI_D3D11
#define VC_RHI_D3D11 0
#endif
#ifndef VC_RHI_METAL
#define VC_RHI_METAL 0
#endif

#if VC_RHI_OPENGL
#include "Device/OpenGL/OpenGLDevice.hpp"
#endif
// #if VC_RHI_VULKAN  #include "Device/Vulkan/VulkanDevice.hpp"  #endif
// #if VC_RHI_D3D11   #include "Device/D3D11/D3D11Device.hpp"    #endif
// #if VC_RHI_METAL   #include "Device/Metal/MetalDevice.hpp"    #endif

namespace VoidCore::RHI
{
    static IRenderDevice *s_device = nullptr;

    uint64_t GetWindowFlags(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
#if VC_RHI_OPENGL
#ifdef NDEBUG
            OpenGL::ConfigureWindowAttributes(false);
#else
            OpenGL::ConfigureWindowAttributes(true);
#endif
            return SDL_WINDOW_OPENGL;
#else
            return 0;
#endif
        case GraphicsAPI::Vulkan: return SDL_WINDOW_VULKAN;
        case GraphicsAPI::Metal:  return SDL_WINDOW_METAL;
        case GraphicsAPI::D3D11:  return 0; // нужен только HWND, флаги не нужны
        }
        return 0;
    }

    std::unique_ptr<IRenderDevice> CreateRenderDevice(GraphicsAPI api)
    {
        switch (api)
        {
#if VC_RHI_OPENGL
        case GraphicsAPI::OpenGL:
            return std::make_unique<OpenGL::OpenGLDevice>();
#endif
        // case GraphicsAPI::Vulkan: return std::make_unique<Vulkan::VulkanDevice>();
        // case GraphicsAPI::D3D11:  return std::make_unique<D3D11::D3D11Device>();
        // case GraphicsAPI::Metal:  return std::make_unique<Metal::MetalDevice>();
        default:
            Debug::LogError("[RHI] Backend not built/implemented: ", ToString(api));
            return nullptr;
        }
    }

    void SetDevice(IRenderDevice *device) { s_device = device; }
    IRenderDevice &GetDevice() { return *s_device; }
}
