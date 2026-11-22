#pragma once
#include <type_traits>
#include <cstddef>

#include <utility>

template <typename T>
class Ref
{
private:
    T *ptr = nullptr;

public:
    Ref() = default;
    Ref(std::nullptr_t) : ptr(nullptr) {}

    Ref(T *p) : ptr(p)
    {
        if (ptr)
            ptr->Retain();
    }

    Ref(const Ref &other) : ptr(other.ptr)
    {
        if (ptr)
            ptr->Retain();
    }

    Ref(Ref &&other) noexcept : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U *, T *>>>
    Ref(const Ref<U> &other) : ptr(other.Get())
    {
        if (ptr)
            ptr->Retain();
    }

    ~Ref()
    {
        if (ptr)
            ptr->Release();
    }

    Ref &operator=(const Ref &other)
    {
        if (this != &other)
        {
            if (ptr)
                ptr->Release();
            ptr = other.ptr;
            if (ptr)
                ptr->Retain();
        }
        return *this;
    }

    Ref &operator=(Ref &&other) noexcept
    {
        if (this != &other)
        {
            if (ptr)
                ptr->Release();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T *Get() const { return ptr; }
    T *operator->() const { return ptr; }
    T &operator*() const { return *ptr; }
    operator T *() const { return ptr; }
    explicit operator bool() const { return ptr != nullptr; }
};
