#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include "../AnimationClip.hpp"
#include "Core/EngineTime.h"

#include "Entity/Components/Base/Component.hpp"

namespace VoidCore
{
    class Animator : public Behaviour
    {
    public:
        AnimationClip *currentClip = nullptr;
        float speed = 1.0f;
        bool playOnStart = true;

        /// @brief Binds a named curve from the active clip to any setter callback.
        template <typename T>
        void Bind(const std::string &propertyName, std::function<void(const T &)> setter)
        {
            m_bindings.push_back([this, propertyName, setter = std::move(setter)](float time)
                                 {
				if (!currentClip)
					return;

				if (const auto* curve = currentClip->GetCurve<T>(propertyName))
				{
					setter(curve->Evaluate(time));
				} });
        }

        /// @brief Removes all property bindings.
        void ClearBindings() { m_bindings.clear(); }

        void Play(AnimationClip *clip = nullptr, bool restart = true)
        {
            if (clip)
                currentClip = clip;
            if (!currentClip)
                return;
            if (restart)
                m_currentTime = 0.0f;
            m_isPlaying = true;
        }

        void Pause() { m_isPlaying = false; }

        void Stop()
        {
            m_isPlaying = false;
            m_currentTime = 0.0f;
        }

        //void Start() override
        //{
        //   if (playOnStart && currentClip)
        //      Play();
        //}

        void Update() override
        {
            if (!m_isPlaying || !currentClip)
                return;

            m_currentTime += static_cast<float>(Time::GetDeltaTime()) * speed;

            if (m_currentTime >= currentClip->duration)
            {
                if (currentClip->isLooping && currentClip->duration > 0.0f)
                {
                    m_currentTime = std::fmod(m_currentTime, currentClip->duration);
                }
                else
                {
                    m_currentTime = currentClip->duration;
                    m_isPlaying = false;
                }
            }

            for (const auto &applyBinding : m_bindings)
            {
                applyBinding(m_currentTime);
            }
        }

    private:
        float m_currentTime = 0.0f;
        bool m_isPlaying = false;
        std::vector<std::function<void(float)>> m_bindings;
    };
}
