#include "Core/Scene.h"
#include "Entity/Components/System/Lighting/Light.hpp"

class GameObject;

Scene::Scene()
{

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

	Camera *activeCamera = nullptr;
	for (GameObject *obj : gameObjects)
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

	for (GameObject *obj : gameObjects)
	{
		auto light = obj->GetComponent<Light>();
		if (light)
		{
			renderer.SubmitLight(light->info);
		}
	}

	for (GameObject *obj : gameObjects)
	{
		if (activeCamera && !activeCamera->IsPointInFrustum(obj->transform->GetPosition()))
			continue;

		MeshRenderer *mr = obj->GetComponent<MeshRenderer>();
		if (!mr || !mr->mesh || !mr->material)
			continue;

		renderer.Submit(mr->mesh, mr->material, obj->transform->WorldMatrix());
	}
}
