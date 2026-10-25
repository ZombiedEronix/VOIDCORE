#pragma once

#include <string>

#include <memory>
#include "Scene.hpp"

#include "LoadSceneMode.hpp"

#include <Tracy/tracy/Tracy.hpp>

class SceneManager
{
public:
	inline static Scene* currentScene = nullptr;

	static void LoadScene(Scene *obj, LoadSceneMode loadMode);

	static void Update();

	static void Render(Renderer& renderer);

	static void UnloadScene();
};