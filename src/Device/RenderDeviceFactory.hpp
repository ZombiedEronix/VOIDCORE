#pragma once
#include "Device/IRenderDevice.hpp"
#include "Device/OpenGL/OpenGLDevice.hpp"
#include <memory>
#include "Device/GraphicsAPI.hpp"

namespace VoidCore::RHI
{

    class RenderDeviceFactory
    {
    public:
        static std::unique_ptr<IRenderDevice> CreateDevice(GraphicsAPI api)
        {
            switch (api)
            {
            case GraphicsAPI::OpenGL:
                return std::make_unique<OpenGL::OpenGLDevice>();
            case GraphicsAPI::Vulkan:
                // Тут в будущем будет return std::make_unique<VulkanDevice>();
                throw std::runtime_error("Vulkan backend not implemented yet!");
            default:
                throw std::runtime_error("Unknown Graphics API!");
            }
        }
    };
}