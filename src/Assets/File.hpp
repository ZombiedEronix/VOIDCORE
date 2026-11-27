#pragma once
#include <vector>
#include <string>
#include <cstdint>

#include "Entity/Object.hpp"

namespace VoidCore::Assets
{
    class File : public Object
    {
    private:
        std::string path;
        std::vector<uint8_t> data;

    public:
        File() = default;

        File(const std::string &filePath, std::vector<uint8_t> &&buffer);

        const std::string &GetPath() const;
        const std::vector<uint8_t> &GetBytes() const;
        const uint8_t *GetData() const;
        size_t GetSize() const;
        bool IsEmpty() const;

        std::string AsString() const;
    };
}