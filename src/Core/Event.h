#pragma once
#include <vector>
#include <functional>

template <typename... Args>
class Event
{
public:
    using CallbackType = std::function<void(Args...)>;

    void Add(CallbackType callback)
    {
        if (callback)
            listeners.push_back(callback);
    }

    void Clear()
    {
        listeners.clear();
    }

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
