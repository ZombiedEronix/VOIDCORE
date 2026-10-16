#include "OpenGLUBO.hpp"

namespace VoidCore::RHI::OpenGL
{
    OpenGLUBO::OpenGLUBO()
    {
        glGenBuffers(1, &m_id);
    }

    OpenGLUBO::~OpenGLUBO()
    {
        glDeleteBuffers(1, &m_id);
    }

    unsigned int OpenGLUBO::GetID() const
    {
        return m_id;
    }

    void OpenGLUBO::Bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    }

    void OpenGLUBO::Unbind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGLUBO::SetData(const void* data, uint32_t size)
    {
        glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
    }
}