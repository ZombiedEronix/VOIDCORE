#pragma once
#include <atomic>
#include <cstdint>

namespace VoidCore::Profiling
{
    inline std::atomic<int64_t> g_liveAllocs{0};
    inline std::atomic<int64_t> g_frameAllocs{0};

    inline void OnAlloc()
    {
        g_liveAllocs.fetch_add(1, std::memory_order_relaxed);
        g_frameAllocs.fetch_add(1, std::memory_order_relaxed);
    }
    inline void OnFree() { g_liveAllocs.fetch_sub(1, std::memory_order_relaxed); }

    struct GLCounters
    {
        std::atomic<int> buffers{0}, textures{0}, samplers{0}, shaders{0}, programs{0}, vaos{0};
    };
    inline GLCounters g_gl;
}
