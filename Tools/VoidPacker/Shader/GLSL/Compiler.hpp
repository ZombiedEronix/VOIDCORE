#pragma once
#include <shaderc/shaderc.hpp>
#include <vector>
#include <string>
#include "Core/Debug.h"

namespace VoidPacker::GLSL
{
    enum class ShaderStage
    {
        Vertex,
        Fragment
    };

    class Compiler
    {
    public:
        static std::vector<uint32_t> CompileShader(const std::string &source, ShaderStage stage, const std::string &debugName = "shader")
        {
            shaderc::Compiler compiler;
            shaderc::CompileOptions options;

            options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
            options.SetGenerateDebugInfo();
            options.SetOptimizationLevel(shaderc_optimization_level_performance);

            shaderc_shader_kind kind = (stage == ShaderStage::Vertex)
                                           ? shaderc_vertex_shader
                                           : shaderc_fragment_shader;

            shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
                source,
                kind,
                debugName.c_str(),
                options);

            if (result.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                Debug::LogError("SPIR-V Compilation error in ", debugName, ":\n", result.GetErrorMessage());
                return {};
            }

            return {result.cbegin(), result.cend()};
        }
    };
}
