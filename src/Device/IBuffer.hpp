#pragma once
#include "Device/IResource.hpp"
#include "Device/RHITypes.hpp"

namespace VoidCore::RHI
{
    struct BufferDesc
    {
        uint64_t size = 0;
        BufferUsage usage = BufferUsage::None;
        MemoryType memory = MemoryType::Static;
        const void *initialData = nullptr; // required for Static, ignored after creation
        const char *debugName = nullptr;
    };

    // One buffer type instead of VBO/EBO/UBO/... - the purpose is specified by usage flags.
    // No Bind()/Unbind(): binding is done via ICommandList.
    class IBuffer : public IResource
    {
    public:
        virtual const BufferDesc &GetDesc() const = 0;
    };
}
