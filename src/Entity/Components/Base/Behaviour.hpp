#pragma once
#include "Component.hpp"

class Behaviour : public Component
{
private:
	bool enabled = true;

public:
	virtual ~Behaviour() = default;

	bool IsEnabled() const { return enabled; }

	void SetEnabled(bool value)
	{
		if (enabled == value)
			return;

		enabled = value;
		if (enabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}

	virtual void OnEnable() {}
	virtual void OnDisable() {}
};
