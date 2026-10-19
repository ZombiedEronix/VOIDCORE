#pragma once

#include "Components/Component.h"
#include "Core/GameObject.h"
#include "Components/Transform.h"
#include "Core/EngineTime.h"

class Rotator : public Component
{
	public:
	float speed = 22.5f;

	void Update() override
	{
		gameObject->transform->SetRotation(gameObject->transform->GetRotation() + glm::vec3(0, Time::deltaTime * speed, 0)); // Rotate by 1 degree per frame
	}
};