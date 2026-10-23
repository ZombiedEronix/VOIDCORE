#pragma once
#include <vector>
#include <functional>

#include "Window.h"
#include "Event.h"

class WindowEvents
{
public:
    static inline Event<Window *, int, int> onResize;
};
