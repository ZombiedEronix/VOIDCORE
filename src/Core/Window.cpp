#include "Window.h"

Window::Window(int w, int h)
{
    width = w;
    height = h;
}

Window::~Window()
{
    Destroy();
}

void Window::Destroy()
{
    if (windowPtr)
    {
        Debug::Log("Shutting down Window & Renderer...");
        if (renderDevice)
            renderDevice->WaitIdle();
        delete renderer; // его GPU-ресурсы должны умереть до устройства
        renderer = nullptr;
        VoidCore::RHI::SetDevice(nullptr);
        renderDevice.reset();
        SDL_DestroyWindow(windowPtr);
        windowPtr = nullptr;
    }
}

void Window::Update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            shouldClose = true;
        }
        else if (event.type == SDL_EVENT_WINDOW_RESIZED)
        {
            width = event.window.data1;
            height = event.window.data2;
            Debug::Log("Window resized to: ", width, "x", height);
        }
    }
}

void Window::EndFrame()
{
    renderDevice->EndFrame();
}

Renderer* Window::GetRenderer() const
{
    return renderer;
}

void Window::SetResolution(int w, int h)
{
    Debug::Log("Setting video resolution to ", w, "x", h);
    width = w;
    height = h;

    if (windowPtr != nullptr && !isFullScreen)
    {
        SDL_SetWindowSize(windowPtr, width, height);
    }
}

void Window::SetFullScreen(bool fullscreen)
{
    Debug::Log("Setting fullscreen mode: ", fullscreen ? "True" : "False");
    isFullScreen = fullscreen;
    SDL_SetWindowFullscreen(windowPtr, fullscreen);
}

bool Window::Initialize(VoidCore::RHI::GraphicsAPI api)
{
    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            Debug::LogError("Failed to initialize SDL: ", SDL_GetError());
            return false;
        }
    }

    windowPtr = SDL_CreateWindow(
        "VoidCore Engine",
        width,
        height,
        VoidCore::RHI::GetWindowFlags(api) | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);

    if (!windowPtr)
    {
        Debug::LogError("Failed to create SDL window: ", SDL_GetError());
        return false;
    }

    renderDevice = VoidCore::RHI::CreateRenderDevice(api);
    if (!renderDevice)
    {
        Debug::LogError("Failed to create render device for ", VoidCore::RHI::ToString(api));
        return false;
    }

    VoidCore::RHI::DeviceDesc deviceDesc;
#ifdef NDEBUG
    deviceDesc.enableValidation = false;
#else
    deviceDesc.enableValidation = true;
#endif
    deviceDesc.vsync = true;

    if (!renderDevice->Initialize(windowPtr, deviceDesc))
    {
        Debug::LogError("Failed to initialize Render Device for window!");
        return false;
    }
    VoidCore::RHI::SetDevice(renderDevice.get());

    renderer = new Renderer();
    Debug::Log("Showing Window");
    return true;
}

void Window::SetResizable(bool enabled)
{
    SDL_SetWindowResizable(windowPtr, enabled);
}

void Window::SetBordered(bool enabled)
{
    SDL_SetWindowBordered(windowPtr, enabled);
}

void Window::SetTitle(const char *name)
{
    SDL_SetWindowTitle(windowPtr, name);
}

void Window::SetMaximumSize(int sizeX, int sizeY)
{
    SDL_SetWindowMaximumSize(windowPtr, sizeX, sizeY);
}

void Window::SetMinimumSize(int sizeX, int sizeY)
{
    SDL_SetWindowMinimumSize(windowPtr, sizeX, sizeY);
}

void Window::SetSize(int sizeX, int sizeY)
{
    SDL_SetWindowSize(windowPtr, sizeX, sizeY);
}

void Window::SetPosition(int sizeX, int sizeY)
{
    SDL_SetWindowPosition(windowPtr, sizeX, sizeY);
}

void Window::SetOpacity(float opacity)
{
    SDL_SetWindowOpacity(windowPtr, opacity);
}

void Window::SetAlwaysOnTop(bool enabled)
{
    SDL_SetWindowAlwaysOnTop(windowPtr, enabled);
}

void Window::Maximize()
{
    SDL_MaximizeWindow(windowPtr);
}

void Window::Minimize()
{
    SDL_MinimizeWindow(windowPtr);
}

void Window::Hide()
{
    SDL_HideWindow(windowPtr);
}

void Window::Show()
{
    SDL_ShowWindow(windowPtr);
}

void Window::Raise()
{
    SDL_RaiseWindow(windowPtr);
}

void Window::Restore()
{
    SDL_RestoreWindow(windowPtr);
}

void Window::SetVerticalSync(bool enabled)
{
    Debug::Log("Setting V-Sync to ", enabled ? "Enabled" : "Disabled");
    if (renderDevice)
        renderDevice->SetVerticalSync(enabled);
}

bool Window::ShouldClose() const
{
    return shouldClose;
}

float Window::GetAspectRatio() const
{
    return ((float)width / (float)height);
}

SDL_Window* Window::GetNativeWindow() const
{
    return windowPtr;
}

VoidCore::RHI::IRenderDevice* Window::GetRenderDevice() const 
{
    return renderDevice.get();
}