#pragma once
#include <vector>
#include <functional>

/// @brief Multicast event dispatcher for subscribing and invoking callbacks.
/// @tparam Args Argument types passed to subscribed listeners.
template <typename... Args>
class Event
{
public:
    using CallbackType = std::function<void(Args...)>;

    /// @brief Subscribes a new callback listener to the event.
    void Add(CallbackType callback)
    {
        if (callback)
            listeners.push_back(callback);
    }

    /// @brief Removes all subscribed listeners.
    void Clear()
    {
        listeners.clear();
    }

    /// @brief Invokes all subscribed listeners with the given arguments.
    void Call(Args... args) const
    {
        for (const auto &callback : listeners)
        {
            callback(args...);
        }
    }

private:
    std::vector<CallbackType> listeners;
};
