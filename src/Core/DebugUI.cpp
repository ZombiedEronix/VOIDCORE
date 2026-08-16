#include "Core/DebugUI.h"

bool DebugUI::initialized = false;
bool DebugUI::enabled = false;

void DebugUI::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Screen::windowPtr, true);
	ImGui_ImplOpenGL3_Init();
	initialized = true;
	enabled = true;
}

void DebugUI::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("MasterDockSpace", nullptr, window_flags);
	ImGui::PopStyleVar(2);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);

	ImGui::End();

	if (enabled && initialized && SceneManager::currentScene != nullptr)
	{
		ImGui::Begin("Scene Hierarchy");
		auto objects = SceneManager::currentScene->gameObjects;
		for (auto& obj : objects)
		{
			std::string label = obj->name;
			if (ImGui::Selectable(label.c_str(), selectedObj == obj))
			{
				selectedObj = obj;
				Debug::Log("Selected : ", selectedObj->name);
			}
		}
		ImGui::End();

		Renderer* renderer = Renderer::Get();

		RenderStats stats;
		if (renderer != nullptr)
		{
			stats = renderer->GetStats();
		}


		ImGui::Begin("RenderStats");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("Draw Calls: %u", stats.drawCalls);
		ImGui::Text("Instances: %u", stats.instanceCount);
		ImGui::Text("Vertices: %u | %u", stats.verticesCount, stats.verticesCount / 3);
		ImGui::End();



		if (selectedObj && selectedObj->transform != nullptr)
		{
			ImGui::Begin("Inspector");
			ImGui::Text("Entity: %s", selectedObj->name.c_str());

			if (selectedObj->transform->GetParent() != nullptr)
			{
				ImGui::Text("Parent: %s", selectedObj->transform->GetParent()->gameObject->name.c_str());
			}

			glm::vec3 pos = selectedObj->transform->GetPosition();
			glm::vec3 rot = selectedObj->transform->GetRotation();
			glm::vec3 scl = selectedObj->transform->GetScale();

			if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
			{
				Debug::Log(pos.x, pos.y, pos.z);
				selectedObj->transform->SetPosition(pos);
			}
			if (ImGui::DragFloat3("Rotation", &rot.x, 1.0f))
			{
				selectedObj->transform->SetRotation(rot);
			}
			if (ImGui::DragFloat3("Scale", &scl.x, 1.0f))
			{
				selectedObj->transform->SetScale(scl);
			}

			if (ImGui::Button("Destroy", ImVec2(0, 0)))
			{
				SceneManager::UnloadScene();

				selectedObj = nullptr;
			}

			ImGui::End();
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}