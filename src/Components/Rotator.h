#pragma once

#include "Components/Component.h"
#include "Core/GameObject.h"
#include "Components/Transform.h"
#include "Core/EngineTime.h"

class Rotator : public Component
{
	void Update() override
	{
		gameObject->transform->SetRotation(glm::vec3(0, Time::deltaTime * 45, 0)); // Rotate by 1 degree per frame
	}
};