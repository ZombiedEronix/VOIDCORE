#include <fstream>
#include <unordered_set>
#include <spirv/spirv_reflect.c>

#include "Shader.hpp"
#include "Core/Debug.h"
#include "Assets/VCSP/VCSP.hpp"
#include <cstring>

using namespace VoidCore::RHI;

void ReflectShaderStage(const void *spirvCode, size_t spirvSize, MaterialBuffer &outLayout)
{
    SpvReflectShaderModule module;
    if (spvReflectCreateShaderModule(spirvSize, spirvCode, &module) != SPV_REFLECT_RESULT_SUCCESS)
        return;

    uint32_t count = 0;
    spvReflectEnumerateDescriptorBindings(&module, &count, nullptr);
    std::vector<SpvReflectDescriptorBinding *> bindings(count);
    spvReflectEnumerateDescriptorBindings(&module, &count, bindings.data());

    for (const auto *b : bindings)
    {
        if (b->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER && b->binding == 1)
        {
            outLayout.binding = b->binding;
            outLayout.totalSize = b->block.size;

            for (uint32_t i = 0; i < b->block.member_count; ++i)
            {
                const auto &member = b->block.members[i];
                if (member.name && member.name[0] != '\0')
                {
                    outLayout.fields[member.name] = {member.offset, member.size};
                }
            }
            break;
        }
    }

    spvReflectDestroyShaderModule(&module);
}

Shader::Shader(const void *data, size_t byteSize, const char *debugName)
{
    auto &device = GetDevice();

    if (!data || byteSize < sizeof(VoidCore::VCSP::Header))
    {
        Debug::LogError("Invalid VCSP data size: ", debugName ? debugName : "unknown");
        return;
    }

    const auto *header = reinterpret_cast<const VoidCore::VCSP::Header *>(data);
    if (std::memcmp(header->magic, "VCSP", 4) != 0 || header->version != 1)
    {
        Debug::LogError("Bad VCSP magic or version in: ", debugName ? debugName : "unknown");
        return;
    }

    if (sizeof(VoidCore::VCSP::Header) + header->vsSize + header->fsSize > byteSize)
    {
        Debug::LogError("Corrupted VCSP section sizes in: ", debugName ? debugName : "unknown");
        return;
    }

    const uint8_t *bytes = static_cast<const uint8_t *>(data);
    const void *vsCode = bytes + sizeof(VoidCore::VCSP::Header);
    const void *fsCode = bytes + sizeof(VoidCore::VCSP::Header) + header->vsSize;

    VoidCore::RHI::ShaderDesc d;
    d.format = VoidCore::RHI::ShaderFormat::SPIRV;
    d.debugName = debugName;

    d.stage = VoidCore::RHI::ShaderStage::Vertex;
    d.code = vsCode;
    d.codeSize = header->vsSize;
    vs = device.CreateShader(d);

    d.stage = VoidCore::RHI::ShaderStage::Fragment;
    d.code = fsCode;
    d.codeSize = header->fsSize;
    fs = device.CreateShader(d);

    ReflectShaderStage(fsCode, header->fsSize, materialLayout);
    ReflectShaderStage(vsCode, header->vsSize, materialLayout);
}

