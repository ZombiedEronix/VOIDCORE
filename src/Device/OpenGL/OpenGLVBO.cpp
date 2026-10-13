#include "OpenGLVBO.hpp"

namespace VoidCore::RHI::OpenGL
{
    OpenGLVBO::OpenGLVBO(const void* data, uint32_t size)
    {
        glGenBuffers(1, &m_id);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    OpenGLVBO::~OpenGLVBO()
    {
        glDeleteBuffers(1, &m_id);
    }

    void OpenGLVBO::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void OpenGLVBO::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    uint32_t OpenGLVBO::GetID() const
    {
        return m_id;
    }

    void OpenGLVBO::SetData(const void *data, uint32_t size)
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        Unbind();
    }
}