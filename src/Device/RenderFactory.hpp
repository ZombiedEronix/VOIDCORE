#pragma once

#include <memory>
#include "Device/IEBO.hpp"
#include "Device/IVBO.hpp"
#include "Device/IVAO.hpp"

#include "OpenGL/OpenGLEBO.hpp"
#include "OpenGL/OpenGLVBO.hpp"
#include "OpenGL/OpenGLVAO.hpp"

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