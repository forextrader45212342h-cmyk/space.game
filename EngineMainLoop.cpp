#include "EngineMainLoop.h"

#include <algorithm>
#include <thread>

namespace Orbit
{
    EngineMainLoop::~EngineMainLoop()
    {
        Shutdown();
    }

    bool EngineMainLoop::Initialize(
        const EngineConfig& InConfig,
        void* NativeWindowHandle)
    {
        Config = InConfig;

        if (Config.TargetFPS != 60 &&
            Config.TargetFPS != 120)
        {
            Config.TargetFPS = 60;
        }

        if (!Physics.Initialize())
        {
            return false;
        }

        if (!Renderer.Initialize(
                Config,
                NativeWindowHandle))
        {
            Physics.Shutdown();
            return false;
        }

        FrameNumber = 0;

        LastFrameTime =
            std::chrono::steady_clock::now();

        bRunning.store(true);

        return true;
    }

    void EngineMainLoop::Run()
    {
        if (!bRunning.load())
        {
            return;
        }

        while (bRunning.load())
        {
            const auto FrameStart =
                std::chrono::steady_clock::now();

            const double DeltaSeconds =
                std::chrono::duration<double>(
                    FrameStart - LastFrameTime
                ).count();

            LastFrameTime = FrameStart;

            const double SafeDelta =
                std::clamp(
                    DeltaSeconds,
                    0.0001,
                    0.25
                );

            Update(SafeDelta);

            Render(SafeDelta);

            ++FrameNumber;

            WaitForFrameBudget(FrameStart);
        }
    }

    void EngineMainLoop::Update(
        double DeltaSeconds)
    {
        /*
         * Engine update order:
         *
         * Input
         *   ↓
         * Gameplay
         *   ↓
         * Physics
         *   ↓
         * Animation
         *
         * Rendering does not directly modify physics state.
         */

        Physics.Simulate(
            DeltaSeconds
        );
    }

    void EngineMainLoop::Render(
        double DeltaSeconds)
    {
        RenderFrame Frame;

        Frame.FrameNumber = FrameNumber;
        Frame.DeltaSeconds = DeltaSeconds;

        Renderer.BeginFrame(Frame);

        Renderer.RenderWorld();

        Renderer.RenderRain();

        Renderer.RenderReflections();

        Renderer.EndFrame();
    }

    void EngineMainLoop::WaitForFrameBudget(
        std::chrono::steady_clock::time_point FrameStart)
    {
        const double TargetFrameTime =
            1.0 /
            static_cast<double>(Config.TargetFPS);

        const auto TargetDuration =
            std::chrono::duration<double>(
                TargetFrameTime
            );

        const auto Now =
            std::chrono::steady_clock::now();

        const auto Elapsed =
            Now - FrameStart;

        if (Elapsed < TargetDuration)
        {
            const auto Remaining =
                TargetDuration - Elapsed;

            /*
             * Sleep is used for coarse pacing.
             * A production platform layer can later add
             * high-resolution waitable timers / spin tails.
             */

            if (Remaining >
                std::chrono::microseconds(500))
            {
                std::this_thread::sleep_for(
                    Remaining -
                    std::chrono::microseconds(250)
                );
            }

            while (
                std::chrono::steady_clock::now() -
                FrameStart <
                TargetDuration)
            {
                std::this_thread::yield();
            }
        }
    }

    void EngineMainLoop::RequestShutdown()
    {
        bRunning.store(false);
    }

    void EngineMainLoop::Shutdown()
    {
        if (bRunning.exchange(false))
        {
            Renderer.Shutdown();
            Physics.Shutdown();
        }
        else
        {
            Renderer.Shutdown();
            Physics.Shutdown();
        }
    }
}
