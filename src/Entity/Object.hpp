#pragma once
#include <string>

class Object
{
private:
    bool m_IsDestroyed = false;

public:
    std::string name;

     static void Destroy(Object *obj)
    {
        if (!obj || obj->IsDestroyed())
            return;
    }

    virtual Object *Clone() const { return nullptr; }

    bool IsDestroyed() const { return m_IsDestroyed; }

    template <typename T>
    static T *Instantiate(const T *original)
    {
        static_assert(std::is_base_of_v<Object, T>, "T must inherit from Object");
        if (!original)
            return nullptr;
        return static_cast<T *>(original->Clone());
    }
};