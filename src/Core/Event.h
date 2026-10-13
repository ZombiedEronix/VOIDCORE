#pragma once
#include <vector>
#include <functional>

template <typename... Args>
class Event
{
    std::vector<std::function<void(Args...)>> listeners;

public:
    void operator+=(std::function<void(Args...)> callback)
    {
        listeners.push_back(callback);
    }

    void Invoke(Args... args)
    {
        for (auto &listener : listeners)
        {
            listener(args...);
        }
    }
};