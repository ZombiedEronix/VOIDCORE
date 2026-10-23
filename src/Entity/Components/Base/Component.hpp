#pragma once

class GameObject;

class Component
{
public:
    GameObject *gameObject = nullptr;

    virtual ~Component() = default;

    virtual void Update() {}
    virtual void Awake() {}
    virtual void OnDestroy() {}
};
