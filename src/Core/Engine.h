#pragma once
#include <memory>
#include <iostream>
#include "Core/Window.h"
#include "Core/SceneManagement/SceneManager.hpp"
#include "Core/EngineTime.h"
#include "Core/Debug.h"
#include "Graphics/Renderer.hpp"
#include "Device/RHI.hpp"

class Engine
{
private:
	static inline bool isRunning = false;
	static void Update();
	static void PrintLogo();

public:
	static bool Initialize();
	static void Run();
	static void ShutDown();
};