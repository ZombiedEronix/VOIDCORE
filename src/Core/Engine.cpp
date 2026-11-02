#include "Engine.h"
#include "Device/RHI.hpp"
#include "Assets/ResourceManager.h"
#include <Tracy/tracy/Tracy.hpp>
#include "Core/Profiling.h"

#include <cstdio>
#include <cstdlib>
#include <new>

#include <malloc.h>
#include <unistd.h>

namespace
{
    double ProcessRssMB()
    {
        long total = 0, resident = 0;
        if (FILE *f = std::fopen("/proc/self/statm", "r"))
        {
            if (std::fscanf(f, "%ld %ld", &total, &resident) != 2)
                resident = 0;
            std::fclose(f);
            return static_cast<double>(resident) * static_cast<double>(sysconf(_SC_PAGESIZE)) / (1024.0 * 1024.0);
        }
        return 0.0;
    }

    void LogMemoryStats(uint64_t frameCounter)
    {
        static uint64_t last = 0;
        static uint64_t lastFrames = 0;
        const uint64_t now = SDL_GetTicks();
        if (now - last < 1000)
            return;
        const uint64_t dt = now - last;
        last = now;

        using namespace VoidCore::Profiling;
        double mallocInUseMB = -1.0;
#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 33)
        mallocInUseMB = static_cast<double>(mallinfo2().uordblks) / (1024.0 * 1024.0);
#endif
        Debug::Log("[MEM] fps=", (frameCounter - lastFrames) * 1000 / (dt ? dt : 1),
                   " rss=", ProcessRssMB(), "MB",
                   " malloc_in_use=", mallocInUseMB, "MB",
                   " newDelete_live=", g_liveAllocs.load(std::memory_order_relaxed),
                   " GL(buf/tex/smp/sh/prg/vao)=", g_gl.buffers.load(), "/", g_gl.textures.load(), "/",
                   g_gl.samplers.load(), "/", g_gl.shaders.load(), "/", g_gl.programs.load(), "/", g_gl.vaos.load());
        lastFrames = frameCounter;
    }

    void PlotMemoryStats()
    {
        static uint64_t frameCounter = 0;
        ++frameCounter;
        LogMemoryStats(frameCounter);

        using namespace VoidCore::Profiling;
        TracyPlot("Heap: live allocs", g_liveAllocs.load(std::memory_order_relaxed));
        TracyPlot("Heap: allocs per frame", g_frameAllocs.exchange(0, std::memory_order_relaxed));
        TracyPlot("GL: buffers", static_cast<int64_t>(g_gl.buffers.load()));
        TracyPlot("GL: textures", static_cast<int64_t>(g_gl.textures.load()));
        TracyPlot("GL: samplers", static_cast<int64_t>(g_gl.samplers.load()));
        TracyPlot("GL: shaders", static_cast<int64_t>(g_gl.shaders.load()));
        TracyPlot("GL: programs", static_cast<int64_t>(g_gl.programs.load()));
        TracyPlot("GL: VAOs", static_cast<int64_t>(g_gl.vaos.load()));

        static uint64_t lastRss = 0;
        const uint64_t now = SDL_GetTicks();
        if (now - lastRss >= 500)
        {
            lastRss = now;
            TracyPlot("Process RSS (MB)", ProcessRssMB());
        }
    }
}



void Engine::Run()
{
	PrintLogo();
	Debug::Log("Initializing VoidCore (C++20)");
	isRunning = true;
	while (isRunning)
	{
		Update();
	}
}

void Engine::Update()
{
	ZoneScopedN("Full Frame");
	Time::Update();
	SceneManager::Update();

	if (Window::IsShowed() == true && Window::IsCreated())
	{
		Window::Update();

		if (Window::ShouldClose())
		{
			Window::Close();
			ShutDown();
			return;
		}

		auto *device = Window::GetRenderDevice();
		Renderer *renderer = Window::GetRenderer();

		renderer->BeginFrame(device->BeginFrame());
		SceneManager::Render(*renderer);

		renderer->Flush();

		device->EndFrame();
	}
	PlotMemoryStats();
	FrameMark;
}

void Engine::PrintLogo()
{
	std::cout << R"(
+=============================================================+
|                                                             |
|                                                             |
|      _    ______  ________     __________  ____  ______     |
|     | |  / / __ \/  _/ __ \   / ____/ __ \/ __ \/ ____/     |
|     | | / / / / // // / / /  / /   / / / / /_/ / __/        |
|     | |/ / /_/ // // /_/ /  / /___/ /_/ / _, _/ /___        |
|     |___/\____/___/_____/   \____/\____/_/ |_/_____/        |
|                                                             |
|                                                             |
+=============================================================+)"
			  << std::endl;
}

void Engine::ShutDown()
{
	Debug::Log("Engine is shutting down.");
	isRunning = false;
	SceneManager::UnloadScene();
	ResourceManager::Clear();
	Debug::Log("GoodBye!");
}

static constexpr int kTraceDepth = 12;

static void *VcAlloc(std::size_t size)
{
    void *p = std::malloc(size ? size : 1);
    if (!p)
        throw std::bad_alloc();
    TracyAllocS(p, size, kTraceDepth);
    VoidCore::Profiling::OnAlloc();
    return p;
}

static void VcFree(void *p) noexcept
{
    if (!p)
        return;
    TracyFreeS(p, kTraceDepth);
    VoidCore::Profiling::OnFree();
    std::free(p);
}

static void *VcAllocAligned(std::size_t size, std::size_t align)
{
    const std::size_t rounded = ((size ? size : 1) + align - 1) / align * align;
    void *p = std::aligned_alloc(align, rounded);
    if (!p)
        throw std::bad_alloc();
    TracyAllocS(p, size, kTraceDepth);
    VoidCore::Profiling::OnAlloc();
    return p;
}

static void VcFreeAligned(void *p) noexcept
{
    if (!p)
        return;
    TracyFreeS(p, kTraceDepth);
    VoidCore::Profiling::OnFree();
    std::free(p);
}

void *operator new(std::size_t n) { return VcAlloc(n); }
void *operator new[](std::size_t n) { return VcAlloc(n); }
void operator delete(void *p) noexcept { VcFree(p); }
void operator delete[](void *p) noexcept { VcFree(p); }
void operator delete(void *p, std::size_t) noexcept { VcFree(p); }
void operator delete[](void *p, std::size_t) noexcept { VcFree(p); }

void *operator new(std::size_t n, std::align_val_t a) { return VcAllocAligned(n, static_cast<std::size_t>(a)); }
void *operator new[](std::size_t n, std::align_val_t a) { return VcAllocAligned(n, static_cast<std::size_t>(a)); }
void operator delete(void *p, std::align_val_t) noexcept { VcFreeAligned(p); }
void operator delete[](void *p, std::align_val_t) noexcept { VcFreeAligned(p); }
void operator delete(void *p, std::size_t, std::align_val_t) noexcept { VcFreeAligned(p); }
void operator delete[](void *p, std::size_t, std::align_val_t) noexcept { VcFreeAligned(p); }
