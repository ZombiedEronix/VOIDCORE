#pragma once
#include <memory>
#include <iostream>
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/EngineTime.h"
#include "Core/Debug.h"
#include "Graphics/RenderPipeline/Renderer.h"
#include "Device/IRenderDevice.hpp"

class Engine {
private:
	bool isRunning;

	Window* window;
	void Update();
	void PrintLogo();
public:
	bool Initialize();
	void Run();
	void ShutDown();

	Window* GetWindow(int id = 0);
	void CreateWindow(int width = 1280, int height = 720);
	void CloseWindow(Window *window);
};