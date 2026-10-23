#include "Window.h"

bool Window::Create(VoidCore::RHI::GraphicsAPI api, int w, int h)
{
    width = w;
    height = h;

    shouldClose = false;
    isFullScreen = false;
    cursor = Cursor();
    
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
        w,
        h,
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
    
    renderer = new Renderer(nullptr);

    Debug::Log("Showing Window");


    isCreated = true;
    return true;
}

float Window::GetWidth()
{
    return width;
}

float Window::GetHeight()
{
    return height;
}

bool Window::IsShowed()
{
    return isShowed;
}

bool Window::IsCreated()
{
    return isCreated;
}

void Window::Close()
{
    Debug::Log("Shutting down Renderer...");
    if (renderDevice)
        renderDevice->WaitIdle();
    delete renderer;
    renderer = nullptr;

    VoidCore::RHI::SetDevice(nullptr);
    renderDevice.reset();

    SDL_DestroyWindow(windowPtr);
    windowPtr = nullptr;
    isCreated = false;
}

Cursor &Window::GetCursor()
{
    return cursor;
}

void Window::Update()
{
    Input::NewFrame();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        Input::ProcessEvent(event);

        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            shouldClose = true;
        }
        else if (event.type == SDL_EVENT_WINDOW_RESIZED)
        {
            pendingWidth = event.window.data1;
            pendingHeight = event.window.data2;
            lastResizeTime = SDL_GetTicks();
            isResizing = true;
        }
    }

    if (isResizing)
    {
        uint64_t currentTime = SDL_GetTicks();
        if (currentTime - lastResizeTime > 150)
        {
            width = pendingWidth;
            height = pendingHeight;

            Debug::Log("Поток ресайза утих. Вызываем тяжелый пересоздание буферов ОДИН раз.");
            OnResize.Call(width, height);

            isResizing = false;
        }
    }
}

void Window::EndFrame()
{
    if (renderDevice)
        renderDevice->EndFrame();
}

Renderer *Window::GetRenderer()
{
    return renderer;
}

void Window::SetResolution(int w, int h)
{
    Debug::Log("Setting video resolution to ", w, "x", h);

    if (windowPtr != nullptr && !isFullScreen)
    {
        width = w;
        height = h;
        SDL_SetWindowSize(windowPtr, w, h);
    }
}

void Window::SetFullScreen(bool fullscreen)
{
    Debug::Log("Setting fullscreen mode: ", fullscreen ? "True" : "False");
    isFullScreen = fullscreen;
    SDL_SetWindowFullscreen(windowPtr, fullscreen);
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

void Window::Hide()
{
    if(isShowed)
    {
        isShowed = false;
        SDL_HideWindow(windowPtr);
    }
}

void Window::Show()
{
    if(!isShowed)
    {
        isShowed = true;
        SDL_ShowWindow(windowPtr);
    }
}

void Window::Minimize()
{
    SDL_MinimizeWindow(windowPtr);
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

bool Window::ShouldClose()
{
    return shouldClose;
}

float Window::GetAspectRatio()
{
    if (height == 0)
        return 1.0f;

    return static_cast<float>(width) / static_cast<float>(height);
}

SDL_Window *Window::GetNativeWindow()
{
    return windowPtr;
}

VoidCore::RHI::IRenderDevice *Window::GetRenderDevice()
{
    return renderDevice.get();
}
