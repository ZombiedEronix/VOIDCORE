#include <cstdint>

namespace VoidCore::RHI
{
    class IUBO
    {
        public:
        IUBO() = default;
        ~IUBO() = default;

        virtual void Bind();
        virtual void Unbind();
        virtual void SetData(const void* data, uint32_t);
    };
}