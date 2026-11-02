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

#include "Entity/Components/System/Camera.hpp"
#include "Entity/Components/System/Lighting/Light.hpp"

#include "Scripts/Rotator.h"
#include "Scripts/Player/FreeCam.hpp"

#include "Entity/Components/System/UI/UICanvas.hpp"
#include "Entity/Components/System/UI/UIDocument.hpp"

#include "UI/Elements.hpp"
#include "UI/UIFactory.hpp"

Engine engine;
Material *material;

#include <random>

static std::shared_ptr<UIControl> BuildDemoUI()
{
	auto rootBackground = std::make_shared<Decorator>();
	auto shader = ResourceManager::GetShader("Assets/shaders/UI/Rectangle");
	auto rootMat = new Material(shader, Material::CreateUIPipeline(shader));

	rootBackground->material = rootMat;
	rootBackground->CornerRadius = 0;
	rootBackground->Margin = 0;
	rootBackground->Background = Color::FromBytes(30, 30, 35, 255);

	auto grid = std::make_shared<Grid>();

	const int numRows = 3;
	const int numCols = 3;

	for (int i = 0; i < numRows; ++i)
	{
		grid->AddRow(1, true);
	}
	for (int j = 0; j < numCols; ++j)
	{
		grid->AddColumn(1, true);
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 255);

	for (int row = 0; row < numRows; ++row)
	{
		for (int col = 0; col < numCols; ++col)
		{
			auto cardBackground = std::make_shared<Decorator>();
			auto cardShader = ResourceManager::GetShader("Assets/shaders/UI/Rectangle");
			auto cardMat = new Material(cardShader, Material::CreateUIPipeline(cardShader));

			cardBackground->Margin = 12;
			cardBackground->CornerRadius = 12;
			cardBackground->material = cardMat;

			uint8_t r = static_cast<uint8_t>(dis(gen));
			uint8_t g = static_cast<uint8_t>(dis(gen));
			uint8_t b = static_cast<uint8_t>(dis(gen));

			cardBackground->Background = Color::FromBytes(r, g, b, 255);

			grid->Add(cardBackground, row, col);
		}
	}

	rootBackground->child = grid;

	return rootBackground;
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
		auto shader = ResourceManager::GetShader("Assets/shaders/Deferred/Lit");
		material = new Material(shader, Material::CreateLitPipeline(shader));

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
		uiObject->GetComponent<UICanvas>()->SetEnabled(true);
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
		Window::GetRenderDevice()->SetVerticalSync(false);
		Window::Show();
	}
	CreateScene();
	engine.Run();
	return 0;
}