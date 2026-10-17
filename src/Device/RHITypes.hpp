#pragma once
// ============================================================================
//  VoidCore RHI — общие типы. Здесь НЕТ ни одного API-специфичного заголовка
//  (никаких glad / vulkan / d3d11 / metal). Если сюда попадёт GLenum — RHI сломан.
// ============================================================================
#include <cstdint>
#include <memory>
#include <type_traits>

namespace VoidCore::RHI
{
    template <typename T>
    using Ref = std::shared_ptr<T>;

    enum class GraphicsAPI : uint8_t
    {
        OpenGL,
        Vulkan,
        D3D11,
        Metal
    };

    inline const char *ToString(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL: return "OpenGL";
        case GraphicsAPI::Vulkan: return "Vulkan";
        case GraphicsAPI::D3D11:  return "Direct3D 11";
        case GraphicsAPI::Metal:  return "Metal";
        }
        return "Unknown";
    }

    // ---- bit-flag helper ----------------------------------------------------
    template <typename E>
    struct EnableBitMask : std::false_type {};

    template <typename E>
        requires EnableBitMask<E>::value
    constexpr E operator|(E a, E b)
    {
        using U = std::underlying_type_t<E>;
        return static_cast<E>(static_cast<U>(a) | static_cast<U>(b));
    }

    template <typename E>
        requires EnableBitMask<E>::value
    constexpr bool HasFlag(E value, E flag)
    {
        using U = std::underlying_type_t<E>;
        return (static_cast<U>(value) & static_cast<U>(flag)) != 0;
    }

    // ---- Форматы ------------------------------------------------------------
    // Берём только то, что есть на ВСЕХ API. Например, RGB8 (3 канала) нет в
    // D3D11/Metal — картинки из stb надо грузить с req_comp = 4.
    enum class Format : uint8_t
    {
        Unknown,

        R8_UNORM,
        RG8_UNORM,
        RGBA8_UNORM,
        RGBA8_SRGB,
        BGRA8_UNORM,

        R16_FLOAT,
        RG16_FLOAT,
        RGBA16_FLOAT,

        R32_FLOAT,
        RG32_FLOAT,
        RGB32_FLOAT, // ТОЛЬКО для вершинных атрибутов (как текстура — нельзя)
        RGBA32_FLOAT,

        R32_UINT,

        D32_FLOAT,
        D24_UNORM_S8_UINT,
    };

    constexpr bool IsDepthFormat(Format f)
    {
        return f == Format::D32_FLOAT || f == Format::D24_UNORM_S8_UINT;
    }

    constexpr uint32_t FormatSize(Format f)
    {
        switch (f)
        {
        case Format::R8_UNORM:          return 1;
        case Format::RG8_UNORM:         return 2;
        case Format::RGBA8_UNORM:
        case Format::RGBA8_SRGB:
        case Format::BGRA8_UNORM:       return 4;
        case Format::R16_FLOAT:         return 2;
        case Format::RG16_FLOAT:        return 4;
        case Format::RGBA16_FLOAT:      return 8;
        case Format::R32_FLOAT:         return 4;
        case Format::RG32_FLOAT:        return 8;
        case Format::RGB32_FLOAT:       return 12;
        case Format::RGBA32_FLOAT:      return 16;
        case Format::R32_UINT:          return 4;
        case Format::D32_FLOAT:         return 4;
        case Format::D24_UNORM_S8_UINT: return 4;
        default:                        return 0;
        }
    }

    // ---- Буферы -------------------------------------------------------------
    enum class BufferUsage : uint32_t
    {
        None    = 0,
        Vertex  = 1 << 0,
        Index   = 1 << 1,
        Uniform = 1 << 2,
    };
    template <> struct EnableBitMask<BufferUsage> : std::true_type {};

    enum class MemoryType : uint8_t
    {
        Static,  // данные только при создании (initialData), потом не меняются
        Dynamic, // CPU обновляет часто (каждый кадр) через ICommandList::UpdateBuffer
    };

    enum class IndexType : uint8_t { UInt16, UInt32 };

    constexpr uint32_t IndexSize(IndexType t) { return t == IndexType::UInt16 ? 2u : 4u; }

    // ---- Текстуры -----------------------------------------------------------
    enum class TextureUsage : uint32_t
    {
        Sampled      = 1 << 0,
        RenderTarget = 1 << 1,
        DepthStencil = 1 << 2,
    };
    template <> struct EnableBitMask<TextureUsage> : std::true_type {};

    enum class Filter : uint8_t { Nearest, Linear };
    enum class AddressMode : uint8_t { Repeat, MirroredRepeat, ClampToEdge };

    // ---- Шейдеры ------------------------------------------------------------
    enum class ShaderStage : uint8_t { Vertex, Fragment };

    // В каком виде лежит код. Каждый бэкенд принимает СВОЙ формат
    // (см. DeviceCaps::shaderFormat), остальное — задача asset-пайплайна.
    enum class ShaderFormat : uint8_t
    {
        GLSL,  // исходник текстом   (OpenGL)
        SPIRV, // бинарник           (Vulkan)
        DXBC,  // бинарник           (D3D11)
        MSL,   // исходник текстом   (Metal)
    };

    // ---- Состояния пайплайна -----------------------------------------------
    enum class PrimitiveTopology : uint8_t { TriangleList, TriangleStrip, LineList, PointList };
    enum class VertexInputRate : uint8_t { PerVertex, PerInstance };
    enum class CullMode : uint8_t { None, Front, Back };

    // Направление обхода задаётся в clip-space/NDC (CCW — как в GLM по умолчанию).
    // Бэкенды, у которых NDC/framebuffer перевёрнуты (Vulkan/D3D/Metal), сами
    // переводят это в своё значение.
    enum class FrontFace : uint8_t { CounterClockwise, Clockwise };

    enum class CompareOp : uint8_t { Never, Less, Equal, LessEqual, Greater, NotEqual, GreaterEqual, Always };

    enum class BlendFactor : uint8_t
    {
        Zero, One,
        SrcColor, OneMinusSrcColor,
        DstColor, OneMinusDstColor,
        SrcAlpha, OneMinusSrcAlpha,
        DstAlpha, OneMinusDstAlpha,
    };
    enum class BlendOp : uint8_t { Add, Subtract, ReverseSubtract, Min, Max };

    enum class ColorWriteMask : uint8_t
    {
        R = 1, G = 2, B = 4, A = 8,
        All = 15
    };
    template <> struct EnableBitMask<ColorWriteMask> : std::true_type {};

    // ---- Render pass --------------------------------------------------------
    enum class LoadOp : uint8_t { Load, Clear, DontCare };

    // ---- Прочее -------------------------------------------------------------
    struct Viewport
    {
        float x = 0, y = 0, width = 0, height = 0;
        float minDepth = 0.0f, maxDepth = 1.0f;
    };

    struct ScissorRect
    {
        int32_t x = 0, y = 0;
        uint32_t width = 0, height = 0;
    };

    // Фиксированные лимиты (одинаковые для всех бэкендов).
    constexpr uint32_t kMaxColorTargets   = 4;
    constexpr uint32_t kMaxVertexBuffers  = 4;
    constexpr uint32_t kMaxUniformBuffers = 8;  // слоты 0..7
    constexpr uint32_t kMaxTextureSlots   = 16; // слоты 0..15
}
