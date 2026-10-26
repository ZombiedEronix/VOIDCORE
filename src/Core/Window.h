#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SDL3/SDL.h>
#include <Core/Debug.h>
#include "Device/RHI.hpp"
#include "Core/Event.h"
#include "Input/Cursor.hpp"
#include "Input/Input.hpp"
#include "Graphics/Renderer.hpp"

class Renderer;

class Window
{
private:
	static inline bool isResizing = false;
	static inline uint64_t lastResizeTime = 0;
	static inline int pendingWidth = 0;
	static inline int pendingHeight = 0;

	static inline Cursor cursor;
	static inline Renderer *renderer = nullptr;
	static inline SDL_Window *windowPtr = nullptr;
	static inline std::unique_ptr<VoidCore::RHI::IRenderDevice> renderDevice = nullptr;
	static inline bool isFullScreen = false;
	static inline bool shouldClose = false;
	static inline int width = 1;
	static inline int height = 1;

	static inline bool isShowed = false;
	static inline bool isCreated = false;

	Window() = delete;
	~Window() = delete;
	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;

public:
	static inline Event<int, int> OnResize;

	static bool Create(VoidCore::RHI::GraphicsAPI API = VoidCore::RHI::GraphicsAPI::OpenGL, int w = 1280, int h = 720);
	static void Destroy();

	static Renderer *GetRenderer();
	static Cursor &GetCursor();

	static void Update();
	static void EndFrame();

	static void SetResolution(int w, int h);
	static void SetFullScreen(bool fullscreen);
	static void SetResizable(bool enabled);
	static void SetBordered(bool enabled);
	static void SetTitle(const char *name);

	static void SetMinimumSize(int sizeX, int sizeY);
	static void SetMaximumSize(int sizeX, int sizeY);
	static void SetSize(int sizeX, int sizeY);
	static void SetPosition(int sizeX, int sizeY);

	static float GetWidth();
	static float GetHeight();

	static void SetOpacity(float opacity);
	static void SetAlwaysOnTop(bool enabled);

	static bool IsCreated();
	static bool IsShowed();

	static void Close();
	static void Minimize();
	static void Maximize();
	static void Hide();
	static void Show();
	static void Raise();
	static void Restore();

	static void SetVerticalSync(bool enabled);

	static bool ShouldClose();
	static float GetAspectRatio();

	static SDL_Window *GetNativeWindow();
	static VoidCore::RHI::IRenderDevice *GetRenderDevice();
};
