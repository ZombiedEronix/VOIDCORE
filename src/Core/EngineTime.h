#pragma once
#include <SDL3/SDL.h>

class Time
{
public:
	/// @brief Returns the scaled frame time in seconds.
	static double GetDeltaTime() { return m_deltaTime; }

	/// @brief Returns the total scaled time since startup in seconds.
	static double GetTime() { return m_time; }

	/// @brief Returns the real (unscaled) frame time in seconds.
	static double GetUnscaledDeltaTime() { return m_unscaledDeltaTime; }

	/// @brief Returns the total real time since startup in seconds.
	static double GetUnscaledTime() { return m_unscaledTime; }

	/// @brief Returns the current time scale multiplier.
	static double GetTimeScale() { return m_timeScale; }

	/// @brief Sets the time scale multiplier (clamped to >= 0.0).
	static void SetTimeScale(double scale) { m_timeScale = scale < 0.0 ? 0.0 : scale; }

	/// @brief Updates all time counters in seconds.
	static void Update()
	{
		double currentTime = static_cast<double>(SDL_GetTicksNS()) / 1'000'000'000.0;

		if (m_isFirstFrame)
		{
			m_lastFrameTime = currentTime;
			m_isFirstFrame = false;
		}

		m_unscaledDeltaTime = currentTime - m_lastFrameTime;
		m_lastFrameTime = currentTime;

		if (m_unscaledDeltaTime > 0.1)
			m_unscaledDeltaTime = 0.1;

		m_unscaledTime += m_unscaledDeltaTime;
		m_deltaTime = m_unscaledDeltaTime * m_timeScale;
		m_time += m_deltaTime;
	}

private:
	inline static double m_deltaTime = 0.0;
	inline static double m_time = 0.0;
	inline static double m_timeScale = 1.0;
	inline static double m_unscaledDeltaTime = 0.0;
	inline static double m_unscaledTime = 0.0;

	inline static double m_lastFrameTime = 0.0;
	inline static bool m_isFirstFrame = true;
};
