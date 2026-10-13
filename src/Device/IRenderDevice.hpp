#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "IVertexBuffer.hpp"
#include "IContext.hpp"

namespace VoidCore::RHI
{
    class IRenderDevice
    {

    public:

        virtual IContext* GetContext() = 0;
        virtual bool Initialize(SDL_Window* window) = 0;
        virtual ~IRenderDevice() = default;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void Clear(float r, float g, float b, float a) = 0;

        virtual void SetVerticalSync(bool enabled) = 0;

        // virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void* data, uint32_t size) = 0;
    };
}
