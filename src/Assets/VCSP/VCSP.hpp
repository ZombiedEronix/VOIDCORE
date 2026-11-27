#include <cstdint>

namespace VoidCore::VCSP
{
#pragma pack(push, 1)
    struct Header
    {
        char magic[4] = {'V', 'C', 'S', 'P'};
        uint32_t version = 1;
        uint32_t vsSize = 0;
        uint32_t fsSize = 0;
    };
#pragma pack(pop)
}