#include <SDL3/SDL.h>

#include "Device/IContext.hpp"

namespace VoidCore::RHI::OpenGL
{

    class OpenGLContext : public IContext
    {
    private:
        SDL_GLContext sdlContext;
        SDL_Window *sdlWindow;

    public:
        OpenGLContext(SDL_Window *window) : sdlWindow(window)
        {
            sdlContext = SDL_GL_CreateContext(sdlWindow);
        }
        ~OpenGLContext()
        {
            SDL_GL_DestroyContext(sdlContext);
        }
        void MakeCurrent() override
        {
            SDL_GL_MakeCurrent(sdlWindow, sdlContext);
        }
        void SwapBuffers() override
        {
            SDL_GL_SwapWindow(sdlWindow);
        }
    };
}