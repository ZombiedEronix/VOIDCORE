#include "Engine.h"
#include <Device/RenderDeviceFactory.hpp>

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
		if(windows.size() > 0)
		{
			Update();
		}
	}
}

void Engine::Update()
{
	Time::Update();

	for (size_t i = 0; i < windows.size(); i++)
	{
		windows[i]->Update();

		if (windows[i]->ShouldClose())
		{
			windows.erase(windows.begin() + i);
			i--;
			continue;
		}

		windows[i]->MakeCurrent();

		// Окно само управляет своим рендерером и отрисовкой!
		Renderer *renderer = windows[i]->GetRenderer();
		renderer->Clear();
		renderer->Update();

		SceneManager::Render(*renderer);

		renderer->Flush();
		windows[i]->EndFrame();
	}
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
+=============================================================+)" << std::endl;
}

Window *Engine::CreateWindow(int width, int height)
{
	auto window = std::make_unique<Window>(width, height);

	Window *rawPtr = window.get();
	windows.push_back(std::move(window));
	return rawPtr;
}

void Engine::ShutDown()
{
	isRunning = false;
	SceneManager::UnloadScene();
	Debug::Log("GoodBye!");
}