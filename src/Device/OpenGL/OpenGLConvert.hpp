#pragma once
// Всё, что превращает RHI-enum'ы в GLenum, живёт ТОЛЬКО здесь.
#include <glad/gl.h>
#include "Device/RHITypes.hpp"

namespace VoidCore::RHI::OpenGL
{
    struct GLTextureFormat
    {
        GLenum internalFormat;
        GLenum format;
        GLenum type;
    };

    inline GLTextureFormat ToGL(Format f)
    {
        switch (f)
        {
        case Format::R8_UNORM:          return {GL_R8, GL_RED, GL_UNSIGNED_BYTE};
        case Format::RG8_UNORM:         return {GL_RG8, GL_RG, GL_UNSIGNED_BYTE};
        case Format::RGBA8_UNORM:       return {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE};
        case Format::RGBA8_SRGB:        return {GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE};
        case Format::BGRA8_UNORM:       return {GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE};
        case Format::R16_FLOAT:         return {GL_R16F, GL_RED, GL_HALF_FLOAT};
        case Format::RG16_FLOAT:        return {GL_RG16F, GL_RG, GL_HALF_FLOAT};
        case Format::RGBA16_FLOAT:      return {GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT};
        case Format::R32_FLOAT:         return {GL_R32F, GL_RED, GL_FLOAT};
        case Format::RG32_FLOAT:        return {GL_RG32F, GL_RG, GL_FLOAT};
        case Format::RGB32_FLOAT:       return {GL_RGB32F, GL_RGB, GL_FLOAT};
        case Format::RGBA32_FLOAT:      return {GL_RGBA32F, GL_RGBA, GL_FLOAT};
        case Format::R32_UINT:          return {GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT};
        case Format::D32_FLOAT:         return {GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT};
        case Format::D24_UNORM_S8_UINT: return {GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8};
        default:                        return {0, 0, 0};
        }
    }

    struct GLVertexFormat
    {
        GLint components;
        GLenum type;
        GLboolean normalized;
        bool isInteger;
    };

    inline GLVertexFormat ToGLVertex(Format f)
    {
        switch (f)
        {
        case Format::R8_UNORM:     return {1, GL_UNSIGNED_BYTE, GL_TRUE, false};
        case Format::RG8_UNORM:    return {2, GL_UNSIGNED_BYTE, GL_TRUE, false};
        case Format::RGBA8_UNORM:
        case Format::RGBA8_SRGB:   return {4, GL_UNSIGNED_BYTE, GL_TRUE, false};
        case Format::R16_FLOAT:    return {1, GL_HALF_FLOAT, GL_FALSE, false};
        case Format::RG16_FLOAT:   return {2, GL_HALF_FLOAT, GL_FALSE, false};
        case Format::RGBA16_FLOAT: return {4, GL_HALF_FLOAT, GL_FALSE, false};
        case Format::R32_FLOAT:    return {1, GL_FLOAT, GL_FALSE, false};
        case Format::RG32_FLOAT:   return {2, GL_FLOAT, GL_FALSE, false};
        case Format::RGB32_FLOAT:  return {3, GL_FLOAT, GL_FALSE, false};
        case Format::RGBA32_FLOAT: return {4, GL_FLOAT, GL_FALSE, false};
        case Format::R32_UINT:     return {1, GL_UNSIGNED_INT, GL_FALSE, true};
        default:                   return {0, 0, GL_FALSE, false};
        }
    }

    inline GLenum ToGL(PrimitiveTopology t)
    {
        switch (t)
        {
        case PrimitiveTopology::TriangleList:  return GL_TRIANGLES;
        case PrimitiveTopology::TriangleStrip: return GL_TRIANGLE_STRIP;
        case PrimitiveTopology::LineList:      return GL_LINES;
        case PrimitiveTopology::PointList:     return GL_POINTS;
        }
        return GL_TRIANGLES;
    }

    inline GLenum ToGL(CompareOp op)
    {
        switch (op)
        {
        case CompareOp::Never:        return GL_NEVER;
        case CompareOp::Less:         return GL_LESS;
        case CompareOp::Equal:        return GL_EQUAL;
        case CompareOp::LessEqual:    return GL_LEQUAL;
        case CompareOp::Greater:      return GL_GREATER;
        case CompareOp::NotEqual:     return GL_NOTEQUAL;
        case CompareOp::GreaterEqual: return GL_GEQUAL;
        case CompareOp::Always:       return GL_ALWAYS;
        }
        return GL_LESS;
    }

    inline GLenum ToGL(BlendFactor f)
    {
        switch (f)
        {
        case BlendFactor::Zero:             return GL_ZERO;
        case BlendFactor::One:              return GL_ONE;
        case BlendFactor::SrcColor:         return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor:         return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha:         return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha:         return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
        }
        return GL_ONE;
    }

    inline GLenum ToGL(BlendOp op)
    {
        switch (op)
        {
        case BlendOp::Add:             return GL_FUNC_ADD;
        case BlendOp::Subtract:        return GL_FUNC_SUBTRACT;
        case BlendOp::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
        case BlendOp::Min:             return GL_MIN;
        case BlendOp::Max:             return GL_MAX;
        }
        return GL_FUNC_ADD;
    }

    inline GLenum ToGL(AddressMode m)
    {
        switch (m)
        {
        case AddressMode::Repeat:         return GL_REPEAT;
        case AddressMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case AddressMode::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        }
        return GL_REPEAT;
    }

    inline GLenum ToGL(IndexType t)
    {
        return t == IndexType::UInt16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
    }

    // min-фильтр в GL — это комбинация filter + mip filter
    inline GLint ToGLMinFilter(Filter min, Filter mip)
    {
        if (min == Filter::Nearest)
            return mip == Filter::Nearest ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_LINEAR;
        return mip == Filter::Nearest ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR;
    }
}
