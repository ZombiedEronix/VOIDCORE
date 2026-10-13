#include "cstdint"

namespace VoidCore::RHI
{
    class IIndexBuffer
    {
    public:
        virtual ~IIndexBuffer() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetCount() const = 0;
    };
}