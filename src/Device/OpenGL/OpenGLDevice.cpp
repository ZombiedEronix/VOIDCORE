#include "OpenGLDevice.hpp"

namespace VoidCore::RHI::OpenGL
{
    bool OpenGLDevice::Initialize(SDL_Window *window)
    {
        windowPtr = window;
        context = new OpenGLContext(window);

        if (!context)
        {
            Debug::LogError("Failed to create OpenGL context: ", SDL_GetError());
            return false;
        }

        context->MakeCurrent();

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            Debug::LogError("Failed to initialize Glad!");
            return false;
        }

        Debug::Log("OpenGL Device Initialized. Version: ", (const char *)glGetString(GL_VERSION));
        return true;
    }

    void OpenGLDevice::BeginFrame()
    {
    }

    void OpenGLDevice::EndFrame()
    {

    }
    
    IContext* OpenGLDevice::GetContext()
    {
        return context;
    }

    void OpenGLDevice::SwapBuffers()
    {
        SDL_GL_SwapWindow(windowPtr);
    }

    void OpenGLDevice::SetVerticalSync(bool enabled)
    {
        SDL_GL_SetSwapInterval(enabled ? 1 : 0);
    }

    void OpenGLDevice::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    OpenGLDevice::~OpenGLDevice()
    {
        delete context;
    }
}