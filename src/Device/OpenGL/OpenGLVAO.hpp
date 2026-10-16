#pragma once
#include "Device/IVAO.hpp"
#include <glad/gl.h>

namespace VoidCore::RHI::OpenGL
{
    class OpenGLVAO : public IVAO
    {
    public:
        OpenGLVAO();
        ~OpenGLVAO() override;
        void Bind() override;
        void Unbind() override;

        void LinkVertexBuffer(IVBO *vbo, uint32_t layout, uint32_t numComponents, uint32_t type, size_t stride, const void *offset) override;

    private:
        unsigned int m_id = 0;
    };
}