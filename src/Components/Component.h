#pragma once
class GameObject;

class Component
{
public:
	GameObject* gameObject = nullptr;
	virtual void Awake() {}
	virtual void Update() {}
	virtual void OnDestroy() {}
	virtual ~Component() = default;
};