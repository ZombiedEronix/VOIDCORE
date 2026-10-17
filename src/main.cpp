#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include "Core/Engine.h"
#include "Assets/ModelImporter.h"
#include "Components/Rotator.h"
#include "Assets/ResourceManager.h"
#include "Core/Window.h"

#include "Components/Camera.h"

#include <chrono>
#include <format>
#include <unordered_map>
#include <Components/Light.h>

#include <SDL3/SDL.h>

#include "tracy/Tracy.hpp"
#include <chrono>
#include <thread>

Engine engine;
Material *material;

void CreateScene()
{
	Scene *scene;
	Mesh *monkeyMesh;

	{
		ZoneScopedN("Mesh");
		monkeyMesh = ResourceManager::GetMesh("Assets/models/Monkey.entm");
		// FrameMark УБРАЛИ
	}

	{
		ZoneScopedN("Creating Material");
		material = new Material();
		material->shader = ResourceManager::GetShader("Assets/shaders/unlitShader");
		material->SetTexture(0, ResourceManager::GetTexture("Assets/textures/test02.jpg", false).get());
		// FrameMark УБРАЛИ
	}

	{
		ZoneScopedN("CreatingScene"); // <-- ТУТ ПОСТАВИЛИ ЗАБЫТУЮ ТОЧКУ С ЗАПЯТОЙ
		scene = new Scene();

		GameObject *cam = new GameObject();

		cam->AddComponent<Camera>(engine.GetWindow());
		cam->name = "Camera";
		cam->transform->SetPosition(glm::vec3(0, 0, -5));

		cam->transform->SetRotation(glm::vec3(0, 90.0f, 0));
		scene->AddGameObject(cam);

		GameObject *pr = new GameObject();
		pr->name = "Papka";
		scene->AddGameObject(pr);

		for (int i = 0; i < 5; i++)
		{
			for (int il = 0; il < 5; il++)
			{
				GameObject *monkey = new GameObject();
				monkey->name = std::format("Object {} {}", i, il);
				monkey->transform->SetScale(glm::vec3(12));
				monkey->AddComponent<MeshRenderer>();
				auto renderer = monkey->GetComponent<MeshRenderer>();
				renderer->SetMesh(monkeyMesh);
				renderer->material = material;

				monkey->transform->SetPosition(glm::vec3(24 * i, 0, 24 * il));
				monkey->transform->SetParent(pr->transform);

				scene->AddGameObject(monkey);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int il = 0; il < 4; il++)
			{
				GameObject *light = new GameObject();
				light->name = std::format("light {} {}", i, il);

				light->AddComponent<Light>();

				light->transform->SetPosition(glm::vec3(24 * i, 1, 24 * il));
				light->transform->SetParent(pr->transform);

				scene->AddGameObject(light);
			}
		}
		// FrameMark УБРАЛИ
	}

	{
		ZoneScopedN("Load Scene");
		SceneManager::LoadScene(scene);
		// FrameMark УБРАЛИ
	}
}

int main(int argc, char *args[])
{
	{
		ZoneScopedN("Initializing Engine");
		engine = Engine();
		engine.Initialize();
	}

	{
		ZoneScopedN("CreatingWindow and context");
		engine.CreateWindow();
		engine.GetWindow()->GetRenderDevice()->SetVerticalSync(true);
		engine.GetWindow()->Show();
	}
	CreateScene();
	engine.Run();
	return 0;
}