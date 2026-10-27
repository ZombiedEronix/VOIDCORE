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

#include "Entity/Components/System/UI/UICanvas.hpp"
#include "Entity/Components/System/UI/UIDocument.hpp"

#include "UI/Elements.hpp"

Engine engine;
Material *material;

static std::shared_ptr<UIControl> BuildDemoUI()
{
	auto root = std::make_shared<Decorator>();
	auto grid = std::make_shared<Grid>();

	root->Name = "root";
	root->Margin = Thickness(24);
	root->Padding = Thickness(16);
	root->HorizontalAlignment = HAlign::Stretch;
	root->VerticalAlignment = VAlign::Stretch;
	root->Background = Color(.11, .22, .15, 0.85f);
	root->BorderColor = Color(0.40f, 0.60f, 1.00f);
	root->BorderThickness = 0.0f;
	root->CornerRadius = 24.0f;

	auto stack = std::make_shared<StackPanel>();
	stack->Spacing = 8.0f;
	root->child = stack;

	stack->Add(std::make_shared<TextBlock>("VoidCore UI", Color(1.0f, 0.85f, 0.3f)));
	stack->Add(std::make_shared<TextBlock>("Test 01"));
	stack->Add(std::make_shared<TextBlock>("Test 02"));

	auto bar = stack->Add(std::make_shared<ProgressBar>());
	bar->Value = 0.65f;

	auto row = stack->Add(std::make_shared<StackPanel>(Orientation::Horizontal, 8.0f));
	for (int i = 1; i <= 3; ++i)
	{
		auto card = row->Add(std::make_shared<Decorator>());
		card->Padding = Thickness(6, 12);
		card->Background = Color(0.25f, 0.30f, 0.50f);
		card->child = std::make_shared<TextBlock>("Card " + std::to_string(i));
	}

	grid->AddColumn(200);
	grid->AddRow(1.0f, true);

	grid->Add(root, 0, 0);
	return grid;
}

void CreateScene()
{
	Scene *scene;
	Mesh *monkeyMesh;

	{
		ZoneScopedN("Mesh");
		monkeyMesh = ResourceManager::GetMesh("Assets/models/Tunnel.entm");
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

		GameObject *uiObject = new GameObject();
		uiObject->name = "UI";
		uiObject->AddComponent<UICanvas>();
		uiObject->AddComponent<UIDocument>();
		uiObject->GetComponent<UICanvas>()->SetDocument(uiObject->GetComponent<UIDocument>());
		uiObject->GetComponent<UIDocument>()->root = BuildDemoUI();

		scene->AddGameObject(uiObject);
	}

	{
		ZoneScopedN("Load Scene");
		SceneManager::LoadScene(scene, LoadSceneMode::Single);
	}
}

int main(int argc, char *args[])
{
	{
		ZoneScopedN("CreatingWindow and context");
		Window::Create();
		Window::GetRenderDevice()->SetVerticalSync(true);
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