#pragma once
#include <cstdint>
#include <memory>
#include "Device/IRenderDevice.hpp"

// The only point where the engine "selects the API". The rest of the code is only visible
// interfaces from IRenderDevice.hpp.
namespace VoidCore::RHI
{
    // Flags for SDL_CreateWindow (SDL_WINDOW_OPENGL / _VULKAN / _METAL / 0).
    // Call BEFORE window creation: The OpenGL backend immediately sets GL attributes.
    uint64_t GetWindowFlags(GraphicsAPI api);

    // nullptr if the backend is not built or implemented.
    std::unique_ptr<IRenderDevice> CreateRenderDevice(GraphicsAPI api);

    // Global access to the active device (Mesh/Texture/Material are created
    // from places where the device cannot be transferred). Window puts it after Initialize.
    void SetDevice(IRenderDevice *device);
    IRenderDevice &GetDevice();
}
