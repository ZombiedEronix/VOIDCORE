#include "Core/SceneManager.h"

void SceneManager::LoadScene(Scene* obj)
{
	ZoneScopedN("SceneManager Load Scene");
	if (currentScene != nullptr)
	{
		UnloadScene();
	}

	Debug::Log("Loading Scene : ", obj->name);
	currentScene = obj;
}

void SceneManager::Update()
{
	ZoneScopedN("SceneManager Update Scene");
	if (currentScene != nullptr)
	{
		currentScene->Update();
	}
}

void SceneManager::Render(Renderer& renderer)
{
	ZoneScopedN("SceneManager Render Scene");
	if (currentScene != nullptr)
	{
		currentScene->Render(renderer);
	}
}

void SceneManager::UnloadScene()
{
	ZoneScopedN("SceneManager Unload Scene");
	if (currentScene != nullptr)
	{
		Debug::Log("Unloading Scene : ", currentScene->name);
		delete currentScene;
		currentScene = nullptr;
	}
}