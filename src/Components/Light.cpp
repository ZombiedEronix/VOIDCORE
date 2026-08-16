#include "Light.h"

LightCommand Light::GetData()
{
	LightCommand command = LightCommand();
	command.position = this->gameObject->transform->GetPosition();
	command.intensity = intensity;
	command.color = color;
	command.radius = radius;

	return command;
}
void Light::Update() {}