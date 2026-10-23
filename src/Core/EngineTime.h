#pragma once
#include <SDL3/SDL.h>

class Time
{
private:
	inline static float m_deltaTime = 0.0f;
	inline static float m_time = 0.0f;
	inline static float m_timeScale = 1.0f;
	inline static float m_unscaledDeltaTime = 0.0f;
	inline static float m_unscaledTime = 0.0f;

	inline static float m_lastFrameTime = 0.0f;
	inline static bool m_isFirstFrame = true;

public:
	inline static float GetDeltaTime() { return m_deltaTime; }
	inline static float GetTime() { return m_time; }
	inline static float GetUnscaledDeltaTime() { return m_unscaledDeltaTime; }
	inline static float GetUnscaledTime() { return m_unscaledTime; }

	inline static float GetTimeScale() { return m_timeScale; }
	inline static void SetTimeScale(float scale) { m_timeScale = scale; }

	static void Update()
	{
		float currentTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;

		if (m_isFirstFrame)
		{
			m_lastFrameTime = currentTime;
			m_isFirstFrame = false;
		}

		m_unscaledDeltaTime = currentTime - m_lastFrameTime;
		m_lastFrameTime = currentTime;

		if (m_unscaledDeltaTime > 0.1f)
			m_unscaledDeltaTime = 0.1f;

		m_unscaledTime += m_unscaledDeltaTime;
		m_deltaTime = m_unscaledDeltaTime * m_timeScale;
		m_time += m_deltaTime;
	}
};