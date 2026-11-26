#include "Core/Engine.h"
#include "Core/Window.h"
#include "Core/SceneManagement/SceneManager.hpp"
#include "Core/SceneManagement/Scene.hpp"

#include "Assets/ResourceManager.h"
#include "Entity/GameObject.hpp"
#include "Entity/Components/System/Camera.hpp"
#include "Entity/Components/System/UI/UICanvas.hpp"
#include "Entity/Components/System/UI/UIDocument.hpp"
#include "UI/Elements.hpp"
#include "Input/Input.hpp"

#include "Viewport/Gizmo.hpp"
#include "Viewport/GizmoPass.hpp"
#include "Graphics/Pass/RenderPassEvent.hpp"
#include "Scripts/FlyCamera.hpp"
#include "Scripts/EditorController.hpp"

using namespace VoidCore;
using namespace VoidCore::SceneManagement;

int main()
{
    if (!ResourceManager::MountPak("gamedata.vpak"))
    {
        Debug::LogError("Failed to mount VPAK!");
        return -1;
    }

    if (!Window::Create(VoidCore::RHI::GraphicsAPI::OpenGL))
        return -1;

    Window::SetTitle("VEditor");

    Window::GetRenderer()->GetRenderPipeline()->AddPass(std::make_unique<VEditor::Rendering::GizmoPass>(), VoidCore::Rendering::RenderPassEvent::BeforeUI);

    Scene *editorScene = new Scene();
    editorScene->name = "EditorScene";

    Ref<GameObject> camObj = new GameObject();
    camObj->name = "EditorCamera";
    camObj->transform->SetPosition({0.0f, 3.0f, 6.0f});
    camObj->transform->SetRotation({-25.0f, -90.0f, 0.0f});
    camObj->AddComponent<FlyCamera>();
    camObj->AddComponent<Camera>();
    camObj->AddComponent<EditorController>()->cam = camObj;

    GameObject *testObj = new GameObject();
    testObj->name = "TestObject";
    editorScene->AddGameObject(camObj);

    SceneManager::LoadScene(editorScene, LoadSceneMode::Single);
    Window::Show();

    VoidCore::Core::Engine::Run();
    return 0;
}
