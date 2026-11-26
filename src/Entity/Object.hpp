#pragma once
#include <cstdint>
#include <string>
#include "GarbageCollector/GC.hpp"
class Object
{
private:
    mutable uint32_t m_RefCount = 0; ///< Object references
    bool m_IsDestroyed = false;      ///< Is the object destroyed
    mutable bool m_InGarbage = false; //< Is it already garbage (To protect against double deletes by the garbage collector)
    void Retain() const { ++m_RefCount; }
    void Release() const
    {
        if (m_RefCount > 0 && --m_RefCount == 0)
        {
            GC::Add(const_cast<Object *>(this));
        }
    }

    friend class GC;
    template <typename T>
    friend class Ref;

public:
    std::string name; ///< Object name

    /// @brief Marks the object as destroyed for safe destruction at the end of the frame
    /// @param obj A pointer to the object being destroyed
    static void Destroy(Object *obj)
    {
        if (!obj || obj->IsDestroyed())
            return;
        obj->m_IsDestroyed = true;
    }

    /// @brief Clones the object.
    /// @return The cloned object
    virtual Object *Clone() const { return nullptr; }

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

    virtual ~Object() = default;
};

/// @brief Cleans up accumulated garbage per frame.
inline void GC::Collect()
{
    for (Object *obj : garbage)
    {
        if (obj->GetRefCount() == 0)
        {
            delete obj;
        }
        else
        {
            obj->m_InGarbage = false;
        }
    }
    garbage.clear();
}