#pragma once

#include "Device/IRenderDevice.hpp"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <Core/Debug.h>

#include "OpenGLDevice.hpp"
#include "Device/IContext.hpp"
#include "Device/OpenGL/OpenGLContext.hpp"

namespace VoidCore::RHI::OpenGL
{
    class OpenGLDevice : public IRenderDevice
    {
    private:
        SDL_GLContext glContext = nullptr;
        SDL_Window *windowPtr = nullptr;
        OpenGLContext* context;


    public:
        IContext* GetContext();
        bool Initialize(SDL_Window *window) override;
        void BeginFrame() override;
        void EndFrame() override;
        void Clear(float r, float g, float b, float a);
        void SwapBuffers();
        ~OpenGLDevice();
        void SetVerticalSync(bool enabled);
    };
}