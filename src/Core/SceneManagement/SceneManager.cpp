#include "SceneManager.hpp"

void SceneManager::LoadScene(Scene *obj, LoadSceneMode loadMode = LoadSceneMode::Single)
{
	ZoneScopedN("SceneManager Load Scene");
	Debug::Log("Loading Scene : ", obj->name);

	if(loadMode == LoadSceneMode::Additive)
	{
		Debug::LogError("Loading scene in additing mode is not implemented");
		return;
	}

	if (currentScene != nullptr)
	{
		UnloadScene();
	}
	
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