#pragma once

#include "Device/IUBO.hpp"
#include <glad/gl.h>

namespace VoidCore::RHI::OpenGL
{
    class OpenGLUBO : public IUBO
    {
    private:
        unsigned int m_id = 0;

    public:
        OpenGLUBO();
        ~OpenGLUBO();
        void Bind() override;
        void Unbind() override;
        unsigned int GetID() const override;
        void SetData(const void *data, uint32_t size) override;
    };
}
