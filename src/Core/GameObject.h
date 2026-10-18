#pragma once
#include <vector>
#include <memory>
#include <string>
#include <typeinfo>

#include "Components/Component.h"
#include "Components/Transform.h"
#include "Tracy/tracy/Tracy.hpp"

class GameObject {
public:
	Transform* transform;
	std::vector<Component*> components;
	std::string name;

	GameObject();
	~GameObject();
	void Update();

	template <typename T>
	T* GetComponent() const;

	template <typename T, typename... Args>
	T* AddComponent(Args&&... args);
};

template <typename T, typename... Args>
T* GameObject::AddComponent(Args&&... args) {
	auto comp = new T(std::forward<Args>(args)...);

	comp->gameObject = this;
	components.push_back(comp);
	comp->Awake();
	return comp;
}

template <typename T>
T* GameObject::GetComponent() const {
	for (auto& comp : components) {
		if (T* casted = dynamic_cast<T*>(comp)) {
			return casted;
		}
	}
	return nullptr;
}