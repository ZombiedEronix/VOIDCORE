#include "GameObject.hpp"
#include "Entity/Components/Base/Behaviour.hpp"
#include "Core/SceneManagement/SceneManager.hpp"

GameObject::GameObject() {
    Transform* t = new Transform();
    t->gameObject = this;
    transform = t;
    components.push_back(t);
}

GameObject *GameObject::Clone() const
{
    GameObject *clone = new GameObject();
    clone->name = this->name + " (Clone)";


    // SceneManager::currentScene->AddGameObject(clone);

    clone->transform->SetPosition(this->transform->GetPosition());
    clone->transform->SetRotation(this->transform->GetRotation());
    clone->transform->SetScale(this->transform->GetScale());

    for (Component *comp : this->components)
    {
        if (comp == this->transform)
            continue;

        if (Component *clonedComp = static_cast<Component *>(comp->Clone()))
        {
            clonedComp->gameObject = clone;
            clone->components.push_back(clonedComp);

            if (auto *origBehaviour = dynamic_cast<Behaviour *>(comp))
            {
                if (auto *cloneBehaviour = dynamic_cast<Behaviour *>(clonedComp))
                {
                    cloneBehaviour->SetEnabled(origBehaviour->IsEnabled());
                }
            }

            clonedComp->Awake();
        }
    }

    for (Transform *childTransform : this->transform->GetChildren())
    {
        GameObject *childClone = childTransform->gameObject->Clone();
        childClone->transform->SetParent(clone->transform);
    }

    return clone;
}


GameObject::~GameObject() {
    for (auto& comp : components) {
        comp->OnDestroy();
        delete comp;
    }
}

/// @brief This method updates the components connected to the object.
///
/// TODO: In this method, all components are updated, without exception, even those that do not have an Update or LateUpdate() implementation.
/// It may be worth trying to remove unnecessary dummy calls, since these calls cannot be cut out by the compiler due to the fact that these are virtual methods.
void GameObject::Update()
{
    ZoneScoped;
    for (auto& comp : components) {
        ZoneTransientN(___compZone, typeid(*comp).name(), true);
        if (Behaviour *behaviour = dynamic_cast<Behaviour *>(comp))
        {
            if (behaviour->IsEnabled())
            {
                behaviour->Update();
            }
        }
    }
}

/// @brief This method updates the components connected to the object after the Update() function.
///
/// TODO: In this method, all components are updated, without exception, even those that do not have an Update or LateUpdate() implementation.
/// It may be worth trying to remove unnecessary dummy calls, since these calls cannot be cut out by the compiler due to the fact that these are virtual methods.
void GameObject::LateUpdate()
{
    ZoneScoped;
    for (auto &comp : components)
    {
        ZoneTransientN(___compZone, typeid(*comp).name(), true);
        if (Behaviour *behaviour = dynamic_cast<Behaviour *>(comp))
        {
            if (behaviour->IsEnabled())
            {
                behaviour->LateUpdate();
            }
        }
    }
}