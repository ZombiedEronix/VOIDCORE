#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SDL3/SDL.h>
#include <Core/Debug.h>
#include "Device/IRenderDevice.hpp"
#include "Device/RenderDeviceFactory.hpp"
#include "Core/Event.h"

#include "Graphics/RenderPipeline/Renderer.h"

class Renderer;

class Window
{
private:
	Renderer *renderer;
	SDL_Window *windowPtr = nullptr;
	std::unique_ptr<VoidCore::RHI::IRenderDevice> renderDevice;
	int width = 1280;
	int height = 720;
	bool isFullScreen = false;
	bool shouldClose = false;

public:
	Window(int w = 1280, int h = 720);
	~Window();

	bool Initialize(VoidCore::RHI::GraphicsAPI API = VoidCore::RHI::GraphicsAPI::OpenGL);

	Renderer* GetRenderer() const;

	void Destroy();
	void Update();

	void EndFrame();
	void MakeCurrent();

	void SetResolution(int w, int h);

	void SetFullScreen(bool fullscreen);

	void SetResizable(bool enabled);
	void SetBordered(bool enabled);
	void SetTitle(const char *name);

	void SetMinimumSize(int sizeX, int sizeY);
	void SetMaximumSize(int sizeX, int sizeY);
	void SetSize(int sizeX, int sizeY);
	void SetPosition(int sizeX, int sizeY);

	void SetOpacity(float opacity);
	void SetAlwaysOnTop(bool enabled);

	void Minimize();
	void Maximize();
	void Hide();
	void Show();
	void Raise();
	void Restore();

	void SetVerticalSync(bool enabled);

	bool ShouldClose() const;
	float GetAspectRatio() const;

	SDL_Window *GetNativeWindow() const;
	VoidCore::RHI::IRenderDevice *GetRenderDevice() const;
};