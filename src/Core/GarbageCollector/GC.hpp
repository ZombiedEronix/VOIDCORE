#pragma once
#include <vector>

class Object;

class GC
{
private:
    inline static std::vector<Object *> garbage;

public:
    static void Add(Object *obj)
    {
        if (obj)
            garbage.push_back(obj);
    }

    static void Collect();
};
