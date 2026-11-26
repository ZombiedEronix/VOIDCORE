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

#include "Animation/Components/Animator.hpp"
#include "Animation/AnimationClip.hpp"
#include "Animation/AnimationCurve.hpp"
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


    Ref<Mesh> mesh = ResourceManager::GetMesh("models/Cube.obj");
    Ref<Material> mat = ResourceManager::GetMaterial("materials/Lit.mat");
    Ref<GameObject> animatedObj = new GameObject();
    MeshRenderer * meshr = animatedObj->AddComponent<MeshRenderer>();
    Animator* animator = animatedObj->AddComponent<Animator>();

    AnimationClip *clip = new AnimationClip();
    clip->duration = 2.0f;
    clip->isLooping = true;

    clip->AddCurve<glm::vec3>("Pos", {{0.0f, {0.0f, 0.0f, 0.0f}, EaseType::EaseInOut},
                                      {1.0f, {0.0f, 2.0f, 0.0f}, EaseType::EaseInOut},
                                      {2.0f, {0.0f, 0.0f, 0.0f}, EaseType::Linear}});

    animator->Bind<glm::vec3>("Pos", [obj = animatedObj.Get()](const glm::vec3 &p)
                              { obj->transform->SetPosition(p); });

    meshr->material = mat;
    meshr->SetMesh(mesh);

    GameObject *testObj = new GameObject();
    testObj->name = "TestObject";
    editorScene->AddGameObject(camObj);
    editorScene->AddGameObject(animatedObj);


    SceneManager::LoadScene(editorScene, LoadSceneMode::Single);
    Window::Show();

    animator->Play(clip);

    VoidCore::Core::Engine::Run();
    return 0;
}
