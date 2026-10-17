#pragma once
#include <cstdint>
#include <memory>
#include "Device/IRenderDevice.hpp"

// Единственная точка, где движок «выбирает API». Остальной код видит только
// интерфейсы из IRenderDevice.hpp.
namespace VoidCore::RHI
{
    // Флаги для SDL_CreateWindow (SDL_WINDOW_OPENGL / _VULKAN / _METAL / 0).
    // Вызывать ДО создания окна: OpenGL-бэкенд тут же выставляет GL-атрибуты.
    uint64_t GetWindowFlags(GraphicsAPI api);

    // nullptr, если бэкенд не собран или не реализован.
    std::unique_ptr<IRenderDevice> CreateRenderDevice(GraphicsAPI api);

    // Глобальный доступ к активному устройству (Mesh/Texture/Material создаются
    // из мест, где device не передашь). Window ставит его после Initialize.
    void SetDevice(IRenderDevice *device);
    IRenderDevice &GetDevice();
}
