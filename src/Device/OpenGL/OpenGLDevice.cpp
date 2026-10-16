#define GLAD_GL_IMPLEMENTATION
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

        int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

        if (version == 0)
        {
            std::cerr << "Пиздец, GLAD 2 не загрузился!" << std::endl;
        }
        else
        {
            std::cout << "Успешно взлетел OpenGL "
                      << GLAD_VERSION_MAJOR(version) << "."
                      << GLAD_VERSION_MINOR(version) << std::endl;
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