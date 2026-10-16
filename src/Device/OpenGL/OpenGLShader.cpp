#include "OpenGLShader.hpp"
#include "Core/Debug.h"

namespace VoidCore::RHI::OpenGL
{
    static GLuint CompileStage(GLenum type, const std::string &source, const char *name)
    {
        GLuint shader = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint ok = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            char log[1024];
            glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
            Debug::LogError(name, " compile error: ", log);
        }
        return shader;
    }

    OpenGLShader::OpenGLShader(const std::string &vertexSource, const std::string &fragmentSource)
    {
        GLuint vs = CompileStage(GL_VERTEX_SHADER, vertexSource, "Vertex");
        GLuint fs = CompileStage(GL_FRAGMENT_SHADER, fragmentSource, "Fragment");

        m_id = glCreateProgram();
        glAttachShader(m_id, vs);
        glAttachShader(m_id, fs);
        glLinkProgram(m_id);

        GLint ok = 0;
        glGetProgramiv(m_id, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            char log[1024];
            glGetProgramInfoLog(m_id, sizeof(log), nullptr, log);
            Debug::LogError("Shader link error: ", log);
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    int OpenGLShader::GetUniformLocation(const std::string &name)
    {
        auto it = m_uniformCache.find(name);
        if (it != m_uniformCache.end())
            return it->second;

        int loc = glGetUniformLocation(m_id, name.c_str());
        m_uniformCache[name] = loc;
        return loc;
    }

    void OpenGLShader::Use()
    {
        glUseProgram(m_id);
    }

    void OpenGLShader::SetInt(const std::string &name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void OpenGLShader::SetFloat(const std::string &name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    unsigned int OpenGLShader::GetID() const
    {
        return m_id;
    }
}
