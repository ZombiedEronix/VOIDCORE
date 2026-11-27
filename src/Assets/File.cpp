#include "File.hpp"

namespace VoidCore::Assets
{
    File::File(const std::string &filePath, std::vector<uint8_t> &&buffer) : path(filePath), data(std::move(buffer)) {}

    const std::string &File::GetPath() const { return path; }
    const std::vector<uint8_t> &File::GetBytes() const { return data; }
    const uint8_t *File::GetData() const { return data.data(); }
    size_t File::GetSize() const { return data.size(); }
    bool File::IsEmpty() const { return data.empty(); }

    std::string File::AsString() const
    {
        return std::string(reinterpret_cast<const char *>(data.data()), data.size());
    }
};