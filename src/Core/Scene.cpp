#include "Scene.h"
#include <Components/Light.h>

class GameObject;

Scene::Scene()
{

}

void Scene::AddGameObject(GameObject* gameObject)
{
	if (gameObject != nullptr)
	{
		gameObjects.push_back(gameObject);
	}
}

void Scene::Update()
{
	if (gameObjects.empty()) return;

	for (auto& obj : gameObjects)
	{
		obj->Update();
	}
}

void Scene::Render(Renderer& renderer) {
	for (GameObject* obj : gameObjects) {
		if (!Camera::main->IsPointInFrustum(obj->transform->GetPosition())) continue;
		
		Light* light = obj->GetComponent<Light>();
		if (light != nullptr)
		{
			renderer.SubmitLight(light->GetData());
		}

		MeshRenderer* mr = obj->GetComponent<MeshRenderer>();
		if (!mr || !mr->mesh || !mr->material) continue;
		
		renderer.Submit(mr->mesh, mr->material, obj->transform->WorldMatrix());
	}

	renderer.Update();
	renderer.Flush();
}