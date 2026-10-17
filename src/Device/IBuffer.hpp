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
        const void *initialData = nullptr; // обязателен для Static, игнорируется после создания
        const char *debugName = nullptr;
    };

    // Один тип буфера вместо VBO/EBO/UBO/... — назначение задаётся usage-флагами.
    // Никаких Bind()/Unbind(): привязка делается через ICommandList.
    class IBuffer : public IResource
    {
    public:
        virtual const BufferDesc &GetDesc() const = 0;
    };
}
