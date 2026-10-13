#pragma once

#include <cstdint>

#include "Device/IVBO.hpp"

namespace VoidCore::RHI
{
    class IVAO
    {
        public:
        uint32_t ID;
        virtual ~IVAO() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void LinkVertexBuffer(IVBO *vbo, uint32_t layout, uint32_t numComponents, uint32_t type, size_t stride, const void *offset) = 0;
    };
}