#pragma once
#include <memory>
#include <iostream>
#include "Core/Screen.h"
#include "Core/SceneManager.h"
#include "Core/EngineTime.h"
#include "Core/Debug.h"
#include "Graphics/RenderPipeline/Renderer.h"




class Engine {
private:
	bool isRunning;
	Renderer* renderer;
	void Update();
	void Render();
	void PrintLogo();
public:
	bool Initialize();
	void Run();
	void ShutDown();
};