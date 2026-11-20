#pragma once
#include <cstdint>

namespace VoidCore::RHI
{
    class IResource
    {
    public:
        virtual ~IResource() = default;
        IResource() = default;
        IResource(const IResource &) = delete;
        IResource &operator=(const IResource &) = delete;

        // For debugging/interop only (ImGui, RenderDoc). Engine for this
        // the value should NOT be based on - it means different things in different APIs.
        virtual uint64_t GetNativeHandle() const = 0;
    };
}
