#include "GameObject.h"

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
    for (auto& comp : components) {
        comp->Update();
    }
}