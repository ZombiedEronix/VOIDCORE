#include "Engine.h"
#include "Device/RHI.hpp"
#include "Assets/ResourceManager.h"
#include "tracy/Tracy.hpp"

bool Engine::Initialize()
{
	PrintLogo();
	Debug::Log("Initializing VoidCore (C++20)");

	return true;
}

void Engine::Run()
{
	isRunning = true;
	while (isRunning)
	{
		Update();
	}
}

void Engine::Update()
{
	ZoneScopedN("Full Frame");
	Time::Update();
	SceneManager::Update();

	if(window != nullptr)
	{
		window->Update();

		if (window->ShouldClose())
		{
			ShutDown();
			return;
		}

		auto *device = window->GetRenderDevice();
		Renderer *renderer = window->GetRenderer();

		renderer->BeginFrame(device->BeginFrame());
		SceneManager::Render(*renderer); // Submit / SubmitLight
		renderer->Flush();               // апдейт буферов + render pass + draw'ы
		device->EndFrame();              // submit + present
	}
	FrameMark;
}

void Engine::PrintLogo()
{
	std::cout << R"(
+=============================================================+
|                                                             |
|                                                             |
|      _    ______  ________     __________  ____  ______     |
|     | |  / / __ \/  _/ __ \   / ____/ __ \/ __ \/ ____/     |
|     | | / / / / // // / / /  / /   / / / / /_/ / __/        |
|     | |/ / /_/ // // /_/ /  / /___/ /_/ / _, _/ /___        |
|     |___/\____/___/_____/   \____/\____/_/ |_/_____/        |
|                                                             |
|                                                             |
+=============================================================+)"
			  << std::endl;
}

void Engine::CreateWindow(int width, int height)
{
	ZoneScoped;
	window = new Window(width, height);
	window->Initialize();
}

Window* Engine::GetWindow(int id)
{
	return window;
}

void Engine::ShutDown()
{
	isRunning = false;
	SceneManager::UnloadScene();

	// GPU-ресурсы должны умереть раньше устройства (его владеет Window).
	ResourceManager::Clear();
	delete window;
	window = nullptr;
	Debug::Log("GoodBye!");
}