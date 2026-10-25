#pragma once
#include <vector>
#include <memory>
#include <string>
#include <typeinfo>

#include "Entity/Components/Base/Behaviour.hpp"
#include "Entity/Components/Base/Component.hpp"
#include "Entity/Components/System/Transform.hpp"
#include "Tracy/tracy/Tracy.hpp"
#include "Entity/Object.hpp"
#include "GameObject.hpp"

class GameObject : public Object
{
private:
	bool isDestroying;
public:
	Transform *transform;
	std::vector<Component *> components;

	GameObject();
	~GameObject();
	void Update();

	bool IsDestroying();

	template <typename T>
	T *GetComponent() const;
	template <typename T, typename... Args>
	T *AddComponent(Args &&...args);
};

template <typename T, typename... Args>
T *GameObject::AddComponent(Args &&...args)
{
	auto comp = new T(std::forward<Args>(args)...);

	comp->gameObject = this;
	components.push_back(comp);
	comp->Awake();
	return comp;
}

template <typename T>
T *GameObject::GetComponent() const
{
	for (auto &comp : components)
	{
		if (T *casted = dynamic_cast<T *>(comp))
		{
			return casted;
		}
	}
	return nullptr;
}