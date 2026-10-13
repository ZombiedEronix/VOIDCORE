#pragma once
#include <cstdint>
#include <cstddef>

namespace VoidCore::RHI
{
    class IVBO
    {
    public:
        virtual ~IVBO() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void SetData(const void *data, uint32_t size) = 0;
        virtual uint32_t GetID() const = 0;
    };
}