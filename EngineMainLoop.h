#pragma once

#include "OrbitTypes.h"
#include "OrbitPhysicsSimulation.h"
#include "OrbitRenderCore.h"

#include <atomic>
#include <chrono>

namespace Orbit
{
    class EngineMainLoop
    {
    public:

        EngineMainLoop() = default;
        ~EngineMainLoop();

        bool Initialize(
            const EngineConfig& Config,
            void* NativeWindowHandle
        );

        void Run();

        void RequestShutdown();

        void Shutdown();

    private:

        void Update(
            double DeltaSeconds
        );

        void Render(
            double DeltaSeconds
        );

        void WaitForFrameBudget(
            std::chrono::steady_clock::time_point FrameStart
        );

    private:

        EngineConfig Config{};

        OrbitPhysicsSimulation Physics;

        OrbitRenderCore Renderer;

        std::atomic<bool> bRunning{false};

        std::uint64_t FrameNumber = 0;

        std::chrono::steady_clock::time_point LastFrameTime;
    };
}
