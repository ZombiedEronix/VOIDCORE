#pragma once
#include <SDL3/SDL.h>

class Time
{
public:
	inline static float deltaTime = 0.0f;
	inline static float time = 0.0f;
	inline static float timeScale = 1.0f;
	inline static float unscaledDeltaTime = 0.0f;
	inline static float unscaledTime = 0.0f;

	static void Update()
	{
		float currentTime = (float)SDL_GetTicks() / 1000.0f;

		unscaledDeltaTime = currentTime - unscaledTime;
		unscaledTime = currentTime;
		time = unscaledTime;

		deltaTime = unscaledDeltaTime * timeScale;
	}
};