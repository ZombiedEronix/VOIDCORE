#pragma once

namespace VoidCore::RHI
{
    class IEBO
    {
    public:
        virtual ~IEBO() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
    };
}