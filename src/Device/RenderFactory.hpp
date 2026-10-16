#pragma once

#include <memory>
#include "Device/IEBO.hpp"
#include "Device/IVBO.hpp"
#include "Device/IVAO.hpp"
#include "Device/IShader.hpp"

#include "OpenGL/OpenGLEBO.hpp"
#include "OpenGL/OpenGLVBO.hpp"
#include "OpenGL/OpenGLVAO.hpp"
#include "OpenGL/OpenGLUBO.hpp"
#include "OpenGL/OpenGLShader.hpp"
#include <string>

#include "Device/GraphicsAPI.hpp"

namespace VoidCore::RHI
{
    class RenderFactory
    {
    public:
        inline static GraphicsAPI CurrentAPI = GraphicsAPI::OpenGL;

        static std::unique_ptr<IEBO> CreateEBO(const void *data, size_t size)
        {
            switch (CurrentAPI)
            {
            case GraphicsAPI::OpenGL:
                return std::make_unique<OpenGL::OpenGLEBO>(data, size);
            default:
                return nullptr;
            }
        }

        static std::unique_ptr<IVBO> CreateVBO(const void *data, size_t size)
        {
            switch (CurrentAPI)
            {
            case GraphicsAPI::OpenGL:
                return std::make_unique<OpenGL::OpenGLVBO>(data, size);
            default:
                return nullptr;
            }
        }

        static std::unique_ptr<IUBO> CreateUBO()
        {
            switch (CurrentAPI)
            {
            case GraphicsAPI::OpenGL:
                return std::make_unique<OpenGL::OpenGLUBO>();
            default:
                return nullptr;
            }
        }

        static std::unique_ptr<IShader> CreateShader(const std::string &vertexSource, const std::string &fragmentSource)
        {
            switch (CurrentAPI)
            {
            case GraphicsAPI::OpenGL:
                return std::make_unique<OpenGL::OpenGLShader>(vertexSource, fragmentSource);
            default:
                return nullptr;
            }
        }

        static std::unique_ptr<IVAO> CreateVAO()
        {
            switch (CurrentAPI)
            {
            case GraphicsAPI::OpenGL:
                return std::make_unique<OpenGL::OpenGLVAO>();
            default:
                return nullptr;
            }
        }
    };
}