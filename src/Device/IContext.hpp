#pragma once

namespace VoidCore::RHI
{
    class IContext
    {
    public:
        virtual ~IContext() = default;
        virtual void MakeCurrent() = 0;
        virtual void SwapBuffers() = 0;
    };
}