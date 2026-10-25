#pragma once
#include "Entity/Object.hpp"

class GameObject;

class Component : Object
{
public:
    GameObject *gameObject = nullptr;

    virtual ~Component() = default;

    virtual void Update() {}
    virtual void Awake() {}
    virtual void OnDestroy() {}
};
