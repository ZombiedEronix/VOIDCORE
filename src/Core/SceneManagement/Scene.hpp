#pragma once

#include <vector>
#include <Tracy/tracy/Tracy.hpp>


#include "Entity/GameObject.hpp"
#include "Entity/Components/System/Camera.hpp"
#include "Entity/Components/System/MeshRenderer.hpp"

#include "Entity/Components/System/UI/UICanvas.hpp"

class Scene {
public:
	std::string name = "Scene";
	std::vector<GameObject*> gameObjects;
	std::vector<UICanvas *> canvases;

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