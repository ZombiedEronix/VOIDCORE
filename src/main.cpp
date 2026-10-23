#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include "Core/Engine.h"
#include "Assets/ModelImporter.h"
#include "Assets/ResourceManager.h"
#include "Core/Window.h"


#include <chrono>
#include <format>
#include <unordered_map>

#include <SDL3/SDL.h>

#include <Tracy/tracy/Tracy.hpp>

#include <chrono>
#include <thread>

#include "Entity/Components/System/Camera.hpp"
#include "Entity/Components/System/Lighting/Light.hpp"


#include "Entity/Components/Scripts/Rotator.h"
#include "Entity/Components/Scripts/FreeCam.hpp"
Engine engine;
Material *material;

void CreateScene()
{
	Scene *scene;
	Mesh *monkeyMesh;

	{
		ZoneScopedN("Mesh");
		monkeyMesh = ResourceManager::GetMesh("Assets/models/monkey.obj");
	}

	{
		ZoneScopedN("Creating Material");
		material = new Material();
		material->shader = ResourceManager::GetShader("Assets/shaders/Deferred/Lit");

		material->SetTexture(0, ResourceManager::GetTexture("Assets/textures/test02_albedo.png", false).get());
		material->SetTexture(1, ResourceManager::GetTexture("Assets/textures/test02_normal.png", false).get());
		material->SetTexture(2, ResourceManager::GetTexture("Assets/textures/test02_ambient.png", false).get());
		material->SetTexture(3, ResourceManager::GetTexture("Assets/textures/test02_specular.png", false).get());
	}

	{
		ZoneScopedN("CreatingScene");
		scene = new Scene();

		GameObject *cam = new GameObject();

		cam->AddComponent<Camera>();
		cam->name = "Camera";
		cam->AddComponent<FreeCam>();
		cam->transform->SetPosition(glm::vec3(-5, 5, -5));
		cam->transform->SetRotation(glm::vec3(-20, 45, 0));

		GameObject *directionalLight = new GameObject();
		directionalLight->name = "Directional Light";
		directionalLight->AddComponent<Light>();

		directionalLight->transform->SetPosition(glm::vec3(glm::vec3(-5, 15, -5)));

		scene->AddGameObject(cam);

		scene->AddGameObject(directionalLight);

		GameObject *pr = new GameObject();
		pr->name = "Papka";
		scene->AddGameObject(pr);

		GameObject *monkey = new GameObject();
		monkey->name = std::format("Обезьяна");
		monkey->transform->SetScale(glm::vec3(3));
		monkey->AddComponent<MeshRenderer>();
		auto renderer = monkey->GetComponent<MeshRenderer>();
		renderer->SetMesh(monkeyMesh);
		renderer->material = material;

		GameObject *light = new GameObject();
		light->name = "Directional Light";
		light->transform->SetRotation(glm::vec3(-80, 0, 0));
		light->AddComponent<Light>();

		GameObject *light2 = new GameObject();
		light2->name = "Directional Light2";
		light2->transform->SetRotation(glm::vec3(80, 90, 0));
		light2->AddComponent<Light>()->color = glm::vec3(1, 0, 0);

		monkey->transform->SetPosition(glm::vec3(0, 0, 0));
		monkey->transform->SetParent(pr->transform);

		monkey->AddComponent<Rotator>();
		scene->AddGameObject(light);
		scene->AddGameObject(light2);
		scene->AddGameObject(monkey);
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
		ZoneScopedN("CreatingWindow and context");
		Window::Create();
		// Window::GetRenderDevice()->SetVerticalSync(true);
		Window::Show();
	}
	CreateScene();
	engine.Run();
	return 0;
}

void *operator new(std::size_t count)
{
	auto ptr = malloc(count);
	TracyAlloc(ptr, count);
	return ptr;
}

void operator delete(void *ptr) noexcept
{
	TracyFree(ptr);
	free(ptr);
}

void *operator new[](std::size_t count)
{
	auto ptr = malloc(count);
	TracyAlloc(ptr, count);
	return ptr;
}

void operator delete[](void *ptr) noexcept
{
	TracyFree(ptr);
	free(ptr);
}