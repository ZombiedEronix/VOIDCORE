#pragma once

#include "glad/gl.h"
#include "Device/IVBO.hpp"

namespace VoidCore::RHI::OpenGL
{
    class OpenGLVBO : public IVBO
    {
    public:
        OpenGLVBO(const void *data, uint32_t size);
        ~OpenGLVBO() override;
        void Bind() override;
        void Unbind() override;
        void SetData(const void *data, uint32_t size) override;
        uint32_t GetID() const override;
    private:
        unsigned int m_id = 0;
    };
}