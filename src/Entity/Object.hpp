#pragma once
#include <cstdint>
#include <string>
#include "GarbageCollector/GC.hpp"
class Object
{
private:
    mutable uint32_t m_RefCount = 0;
    bool m_IsDestroyed = false;

public:
    std::string name;
    virtual ~Object() = default;
    static void Destroy(Object *obj)
    {
        if (!obj || obj->IsDestroyed())
            return;
    }

    virtual Object *Clone() const { return nullptr; }

    void Retain() const { ++m_RefCount; }
    void Release() const
    {
        if (m_RefCount > 0 && --m_RefCount == 0)
        {
            GC::Add(const_cast<Object *>(this));
        }
    }
    uint32_t GetRefCount() const { return m_RefCount; }

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

inline void GC::Collect()
{
    for (Object *obj : garbage)
    {
        if (obj->GetRefCount() == 0)
        {
            delete obj;
        }
    }
    garbage.clear();
}