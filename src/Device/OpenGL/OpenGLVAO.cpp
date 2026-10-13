#include "OpenGLVAO.hpp"
#include "OpenGLVBO.hpp"
#include "Device/IVAO.hpp"
#include "Device/IVBO.hpp"

#include <glad/glad.h>

namespace VoidCore::RHI::OpenGL
{
    OpenGLVAO::OpenGLVAO()
    {
        glGenVertexArrays(1, &m_id);
        Bind();
    }

    OpenGLVAO::~OpenGLVAO()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void OpenGLVAO::Bind()
    {
        glBindVertexArray(m_id);
    }

    void OpenGLVAO::Unbind()
    {
        glBindVertexArray(0);
    }

    void OpenGLVAO::LinkVertexBuffer(IVBO *vbo, uint32_t layout, uint32_t numComponents, uint32_t type, size_t stride, const void *offset)
    {
        auto glVBO = dynamic_cast<OpenGLVBO *>(vbo);
        if (glVBO)
        {
            glVBO->Bind();
            glVertexAttribPointer(layout, numComponents, type, GL_FALSE, static_cast<GLsizei>(stride), offset);
            glEnableVertexAttribArray(layout);
            glVBO->Unbind();
        }
    }
}