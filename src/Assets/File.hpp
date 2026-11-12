#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace VoidCore::Assets
{
    class File
    {
    private:
        std::string path;
        std::vector<uint8_t> data;

    public:
        File() = default;

        File(const std::string &filePath, std::vector<uint8_t> &&buffer)
            : path(filePath), data(std::move(buffer))
        {
        }

        const std::string &GetPath() const { return path; }
        const std::vector<uint8_t> &GetBytes() const { return data; }
        const uint8_t *GetData() const { return data.data(); }
        size_t GetSize() const { return data.size(); }
        bool IsEmpty() const { return data.empty(); }

        std::string AsString() const
        {
            return std::string(reinterpret_cast<const char *>(data.data()), data.size());
        }
    };
}