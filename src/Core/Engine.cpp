#include "Engine.h"
#include "DebugUI.h"

bool Engine::Initialize()
{
	PrintLogo();
	Debug::Log("Initializing Entropy Engine (C++20)");

	if (Screen::Initialize() != 0) return false;
	DebugUI::Initialize();
	return true;
}

void Engine::Run()
{
	isRunning = true;
	renderer = new Renderer();

	while (isRunning)
	{
		Update();
		Render();

		if (Screen::isWindowShouldClose()) isRunning = false;
	}
}

void Engine::Update()
{
	Time::Update();
	SceneManager::Update();
	DebugUI::Update();
	Screen::Update();
}

void Engine::PrintLogo()
{
	std::cout << R"(
       _______  ____________  ____  ______  __
      / ____/ | / /_  __/ __ \/ __ \/ __ \ \/ /
     / __/ /  |/ / / / / /_/ / / / / /_/ /\  / 
    / /___/ /|  / / / / _, _/ /_/ / ____/ / /  
   /_____/_/ |_/ /_/ /_/ |_|\____/_/     /_/   
    )" << std::endl;
}

void Engine::Render()
{
	renderer->Clear();
	SceneManager::Render(*renderer);
}

void Engine::ShutDown()
{
	isRunning = false;
	SceneManager::UnloadScene();
	Screen::Destroy();
	Debug::Log("GoodBye!");
}