#pragma once
#include <glfw3.h>
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
		time = glfwGetTime();
		unscaledDeltaTime = time - unscaledTime;
		unscaledTime = time;
		deltaTime = unscaledDeltaTime * timeScale;
	}
};