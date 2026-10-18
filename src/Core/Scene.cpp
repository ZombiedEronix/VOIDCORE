#include "Core/Scene.h"
#include <Components/Light.h>

class GameObject;

static bool s_JoltInitialized = false;

Scene::Scene()
{
	if (!s_JoltInitialized)
	{
		s_JoltInitialized = true;
	}
}
void Scene::AddGameObject(GameObject *gameObject)
{
	if (gameObject != nullptr)
	{
		gameObjects.push_back(gameObject);
	}
}

void Scene::Update()
{
	ZoneScopedN("Update Scene");

	if (gameObjects.empty())
		return;

	for (auto &obj : gameObjects)
	{
		ZoneTransientN(___objectZone, obj->name.c_str(), true);
		obj->Update();
	}
}

void Scene::Render(Renderer &renderer)
{
	ZoneScoped;
	for (GameObject *obj : gameObjects)
	{
		Camera *activeCamera = nullptr;
		for (auto obj : gameObjects) // или как у тебя хранится список объектов в Scene
		{
			auto cam = obj->GetComponent<Camera>();
			if (cam)
			{
				activeCamera = cam;
				break;
			}
		}

		if (activeCamera)
		{
			renderer.SetCamera(activeCamera);
		}

		if (activeCamera && !activeCamera->IsPointInFrustum(obj->transform->GetPosition()))
			continue;

		MeshRenderer *mr = obj->GetComponent<MeshRenderer>();
		if (!mr || !mr->mesh || !mr->material)
			continue;

		renderer.Submit(mr->mesh, mr->material, obj->transform->WorldMatrix());
	}
	// Загрузка буферов и сам render pass делает Renderer::Flush(), его вызывает Engine.
}