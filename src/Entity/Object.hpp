#pragma once
#include <string>

class Object
{
    public:
    std::string name;

    static void Destroy(Object* obj);
    static void Instantiate(Object* obj);
};