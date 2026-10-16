#pragma once

#include <vector>
#include <Core/GameObject.h>
#include <Components/Camera.h>
#include <Components/MeshRenderer.h>
#include "tracy/Tracy.hpp"

class Scene {
public:
	std::string name = "Scene";
	std::vector<GameObject*> gameObjects;

	Scene();
	~Scene()
	{
		for (auto obj : gameObjects)
		{
			delete obj;
		}
		gameObjects.clear();
	}
	void AddGameObject(GameObject* gameObject);
	void Update();
	void Render(Renderer& renderer);
};