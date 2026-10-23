#pragma once

#include "Entity/Components/Base/Component.hpp"
#include "Entity/GameObject.hpp"
#include "Entity/Components/System/Transform.hpp"
#include "Core/EngineTime.h"

class Rotator : public Behaviour
{
	public:
	float speed = 22.5f;

	void Update() override
	{
		gameObject->transform->SetRotation(gameObject->transform->GetRotation() + glm::vec3(0, Time::deltaTime * speed, 0)); // Rotate by 1 degree per frame
	}
};