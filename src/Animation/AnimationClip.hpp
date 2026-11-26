#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "AnimationCurve.hpp"

namespace VoidCore
{
    /// @brief Asset containing named animation curves for arbitrary properties.
    class AnimationClip
    {
    public:
        std::string name = "New Clip";
        float duration = 1.0f;
        bool isLooping = true;

        /// @brief Adds or replaces a typed curve under the given property name.
        template <typename T>
        AnimationCurve<T> &AddCurve(const std::string &propertyName, std::initializer_list<Keyframe<T>> keys = {})
        {
            auto curve = std::make_shared<AnimationCurve<T>>(keys);
            m_curves[propertyName] = curve;
            return *curve;
        }

        /// @brief Retrieves a typed curve by property name (returns nullptr if missing or type mismatches).
        template <typename T>
        const AnimationCurve<T> *GetCurve(const std::string &propertyName) const
        {
            auto it = m_curves.find(propertyName);
            if (it == m_curves.end())
                return nullptr;

            return dynamic_cast<const AnimationCurve<T> *>(it->second.get());
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<ICurve>> m_curves;
    };
}
