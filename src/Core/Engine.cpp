#include "Engine.h"
#include <Device/RenderDeviceFactory.hpp>
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
		}
		
		Renderer *renderer = window->GetRenderer();
		renderer->Clear();
		renderer->Update();
		
		SceneManager::Render(*renderer);
		
		renderer->Flush();
		window->EndFrame();
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
	Debug::Log("GoodBye!");
}