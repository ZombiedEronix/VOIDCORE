#include "GameObject.hpp"
#include "Entity/Components/Base/Behaviour.hpp"

GameObject::GameObject() {
    Transform* t = new Transform();
    t->gameObject = this;
    transform = t;
    components.push_back(t);
}

GameObject::~GameObject() {
    for (auto& comp : components) {
        comp->OnDestroy();
        delete comp;
    }
}

void GameObject::Update() {
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