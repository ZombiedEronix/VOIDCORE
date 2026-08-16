#pragma once

#include <memory>
#include <Core/Scene.h>

class SceneManager
{
public:
	inline static Scene* currentScene = nullptr;

	static void LoadScene(Scene* obj);

	static void Update();

	static void Render(Renderer& renderer);

	static void UnloadScene();
};