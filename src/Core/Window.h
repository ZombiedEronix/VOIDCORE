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
public:
	static inline Event<int, int> OnResize; ///< Invoked when the window is resized (width, height).

	/// @brief Creates the window and initializes the graphics context.
	/// @param API Graphics backend API to use.
	/// @param w Initial window width in pixels.
	/// @param h Initial window height in pixels.
	/// @return True if the window was created successfully.
	static bool Create(VoidCore::RHI::GraphicsAPI API = VoidCore::RHI::GraphicsAPI::OpenGL, int w = 1280, int h = 720);

	/// @brief Destroys the window and releases associated graphics resources.
	static void Destroy();

	/// @brief Returns the active renderer instance.
	static Renderer *GetRenderer();

	/// @brief Returns a reference to the window cursor controller.
	static Cursor &GetCursor();

	/// @brief Polls window events and updates internal state.
	static void Update();

	/// @brief Swaps buffers and finalizes the current frame.
	static void EndFrame();

	/// @brief Sets the window resolution in pixels.
	static void SetResolution(int w, int h);

	/// @brief Toggles fullscreen mode.
	static void SetFullScreen(bool fullscreen);

	/// @brief Enables or disables user window resizing.
	static void SetResizable(bool enabled);

	/// @brief Enables or disables the window border and title bar.
	static void SetBordered(bool enabled);

	/// @brief Sets the window title text.
	static void SetTitle(const char *name);

	/// @brief Sets the minimum allowed window size in pixels.
	static void SetMinimumSize(int sizeX, int sizeY);

	/// @brief Sets the maximum allowed window size in pixels.
	static void SetMaximumSize(int sizeX, int sizeY);

	/// @brief Sets the current window size in pixels.
	static void SetSize(int sizeX, int sizeY);

	/// @brief Sets the window position on screen.
	static void SetPosition(int sizeX, int sizeY);

	/// @brief Returns the current window width in pixels.
	static float GetWidth();

	/// @brief Returns the current window height in pixels.
	static float GetHeight();

	/// @brief Sets the window opacity (0.0 = transparent, 1.0 = opaque).
	static void SetOpacity(float opacity);

	/// @brief Keeps the window above all other normal windows when enabled.
	static void SetAlwaysOnTop(bool enabled);

	/// @brief Checks whether the window has been created.
	static bool IsCreated();

	/// @brief Checks whether the window is currently visible.
	static bool IsShowed();

	/// @brief Requests the window to close.
	static void Close();

	/// @brief Minimizes the window to the taskbar.
	static void Minimize();

	/// @brief Maximizes the window to fill the screen.
	static void Maximize();

	/// @brief Hides the window.
	static void Hide();

	/// @brief Shows the window if hidden.
	static void Show();

	/// @brief Brings the window to the front and focuses it.
	static void Raise();

	/// @brief Restores a minimized or maximized window to its normal size.
	static void Restore();

	/// @brief Enables or disables vertical synchronization (V-Sync).
	static void SetVerticalSync(bool enabled);

	/// @brief Checks whether the window has received a close request.
	static bool ShouldClose();

	/// @brief Returns the window width-to-height aspect ratio.
	static float GetAspectRatio();

	/// @brief Returns the underlying native SDL_Window pointer.
	static SDL_Window *GetNativeWindow();

	/// @brief Returns the active RHI render device.
	static VoidCore::RHI::IRenderDevice *GetRenderDevice();

private:
	static inline bool isResizing = false;
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
};
