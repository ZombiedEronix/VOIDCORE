#include "Core/SceneManager.h"

void SceneManager::LoadScene(Scene* obj)
{
	if (currentScene != nullptr)
	{
		UnloadScene();
	}

	Debug::Log("Loading Scene : ", obj->name);
	currentScene = obj;
}

void SceneManager::Update()
{
	if (currentScene != nullptr)
	{
		currentScene->Update();
	}
}

void SceneManager::Render(Renderer& renderer)
{
	if (currentScene != nullptr)
	{
		currentScene->Render(renderer);
	}
}

void SceneManager::UnloadScene()
{
	if (currentScene != nullptr)
	{
		Debug::Log("Unloading Scene : ", currentScene->name);
		delete currentScene;
		currentScene = nullptr;
	}
}