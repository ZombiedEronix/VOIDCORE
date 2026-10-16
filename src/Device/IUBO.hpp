#pragma once
#include <cstdint>

namespace VoidCore::RHI
{
    class IUBO
    {
    public:
        virtual ~IUBO() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual unsigned int GetID() const = 0;
        virtual void SetData(const void *data, uint32_t size) = 0;
    };
}
