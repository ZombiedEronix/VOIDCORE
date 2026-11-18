#pragma once
#include <vector>
#include <cstring>
#include "Assets/VCSP/Header.hpp"
namespace VoidPacker::SPIRV
{
    class Builder
    {
    public:
        static std::vector<uint8_t> BuildVCSPBinary(const std::vector<uint32_t> &vsSpirv, const std::vector<uint32_t> &fsSpirv)
        {
            VoidCore::VCSP::Header header;
            header.vsSize = static_cast<uint32_t>(vsSpirv.size() * sizeof(uint32_t));
            header.fsSize = static_cast<uint32_t>(fsSpirv.size() * sizeof(uint32_t));

            std::vector<uint8_t> outBuffer(sizeof(header) + header.vsSize + header.fsSize);

            std::memcpy(outBuffer.data(), &header, sizeof(header));
            std::memcpy(outBuffer.data() + sizeof(header), vsSpirv.data(), header.vsSize);
            std::memcpy(outBuffer.data() + sizeof(header) + header.vsSize, fsSpirv.data(), header.fsSize);

            return outBuffer;
        }
    };
}