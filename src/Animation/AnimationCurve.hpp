#pragma once
#include <vector>
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace VoidCore
{
    /// @brief Interpolation curve type between the current keyframe and the next one.
    enum class EaseType
    {
        Linear,   ///< Constant speed
        Step,     ///< Instant jump to the next keyframe value (no interpolation)
        EaseIn,   ///< Starts slow, accelerates
        EaseOut,  ///< Starts fast, decelerates
        EaseInOut ///< Smooth acceleration and deceleration (SmoothStep)
    };

    struct ICurve
    {
        virtual ~ICurve() = default;
    };

    template <typename T>
    struct Keyframe
    {
        float time = 0.0f;
        T value{};
        EaseType ease = EaseType::Linear;
    };

    /// @brief Generic keyframe curve that interpolates values of type T over time.
    template <typename T>
    class AnimationCurve : public ICurve
    {
    public:
        std::vector<Keyframe<T>> keys;

        AnimationCurve() = default;
        AnimationCurve(std::initializer_list<Keyframe<T>> initKeys) : keys(initKeys) {}

        /// @brief Evaluates the interpolated value at the given timestamp.
        T Evaluate(float time) const
        {
            if (keys.empty())
                return T{};

            if (keys.size() == 1 || time <= keys.front().time)
                return keys.front().value;

            if (time >= keys.back().time)
                return keys.back().value;

            for (size_t i = 0; i < keys.size() - 1; ++i)
            {
                if (time >= keys[i].time && time <= keys[i + 1].time)
                {
                    if (keys[i].ease == EaseType::Step)
                        return keys[i].value;

                    float span = keys[i + 1].time - keys[i].time;
                    float t = (span > 0.0001f) ? (time - keys[i].time) / span : 0.0f;
                    t = ApplyEasing(t, keys[i].ease);

                    return Interpolate(keys[i].value, keys[i + 1].value, t);
                }
            }

            return keys.back().value;
        }

    private:
        static float ApplyEasing(float t, EaseType ease)
        {
            switch (ease)
            {
            case EaseType::EaseIn:
                return t * t;
            case EaseType::EaseOut:
                return 1.0f - (1.0f - t) * (1.0f - t);
            case EaseType::EaseInOut:
                return t * t * (3.0f - 2.0f * t);
            case EaseType::Linear:
            default:
                return t;
            }
        }

        static T Interpolate(const T &a, const T &b, float t)
        {
            if constexpr (std::is_same_v<T, glm::quat>)
                return glm::slerp(a, b, t);
            else
                return glm::mix(a, b, t);
        }
    };
}
