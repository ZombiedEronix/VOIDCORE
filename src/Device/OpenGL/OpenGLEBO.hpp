#pragma once

#include "Device/IEBO.hpp"
#include <glad/glad.h>

namespace VoidCore::RHI::OpenGL
{
    class OpenGLEBO : public IEBO
    {
    public:
        OpenGLEBO(const void *data, GLsizeiptr size);
        ~OpenGLEBO() override;

        void Bind() override;
        void Unbind() override;

    private:
        unsigned int m_id = 0;
    };
}