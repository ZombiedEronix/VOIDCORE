#include "OpenGLEBO.hpp"

namespace VoidCore::RHI::OpenGL
{
    OpenGLEBO::OpenGLEBO(const void *data, GLsizeiptr size)
    {
        glGenBuffers(1, &m_id);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    OpenGLEBO::~OpenGLEBO()
    {
        glDeleteBuffers(1, &m_id);
    }

    void OpenGLEBO::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void OpenGLEBO::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}