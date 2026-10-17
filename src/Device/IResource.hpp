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

        // Только для отладки/интеропа (ImGui, RenderDoc). Движок на это
        // значение опираться НЕ должен — оно значит разное в разных API.
        virtual uint64_t GetNativeHandle() const = 0;
    };
}
