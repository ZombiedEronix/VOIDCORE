#pragma once
#include "cstdint"

namespace VoidCore::RHI
{
    class IVertexBuffer
    {
    public:
        virtual ~IVertexBuffer() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void SetData(const void *data, uint32_t size) = 0;
    };
}