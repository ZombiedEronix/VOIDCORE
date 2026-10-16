#pragma once
#include <string>
#include <unordered_map>
#include <glad/gl.h>
#include "Device/IShader.hpp"

namespace VoidCore::RHI::OpenGL
{
    class OpenGLShader : public IShader
    {
    private:
        unsigned int m_id = 0;
        std::unordered_map<std::string, int> m_uniformCache;

        int GetUniformLocation(const std::string &name);

    public:
        OpenGLShader(const std::string &vertexSource, const std::string &fragmentSource);
        ~OpenGLShader() override = default;

        void Use() override;
        void SetInt(const std::string &name, int value) override;
        void SetFloat(const std::string &name, float value) override;
        unsigned int GetID() const override;
    };
}
