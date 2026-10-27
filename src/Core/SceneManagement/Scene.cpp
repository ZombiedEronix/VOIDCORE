#include "Scene.hpp"
#include "Entity/Components/System/Lighting/Light.hpp"
#include "Entity/Components/System/UI/UICanvas.hpp"

#include <algorithm>

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
		if (activeCamera == nullptr)
			continue;
		if (!activeCamera->IsPointInFrustum(obj->transform->GetPosition()))
		{
			Debug::Log("Скип");
			continue;
		}
		MeshRenderer *mr = obj->GetComponent<MeshRenderer>();
		if (!mr || !mr->mesh || !mr->material)
			continue;

		renderer.Submit(mr->mesh, mr->material, obj->transform->WorldMatrix());
	}

	// ---- UI: рисуется после всей 3D-сцены, канвасы по sortingOrder ----
	std::vector<UICanvas *> canvases;
	for (GameObject *obj : gameObjects)
	{
		if (auto *canvas = obj->GetComponent<UICanvas>())
			canvases.push_back(canvas);
	}
	std::stable_sort(canvases.begin(), canvases.end(),
					 [](const UICanvas *a, const UICanvas *b)
					 { return a->sortingOrder < b->sortingOrder; });

	for (UICanvas *canvas : canvases)
		canvas->Render(renderer.GetUIContext(), Window::GetWidth(), Window::GetHeight());
}
