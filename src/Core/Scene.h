#pragma once

#include <vector>
#include <Tracy/tracy/Tracy.hpp>


#include "Entity/GameObject.hpp"
#include "Entity/Components/System/Camera.hpp"
#include "Entity/Components/System/MeshRenderer.hpp"

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