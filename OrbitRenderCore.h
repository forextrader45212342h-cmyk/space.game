#pragma once

#include "OrbitTypes.h"

#include <cstdint>
#include <memory>

namespace Orbit
{
    enum class RenderBackend
    {
        None,
        DirectX12,
        Vulkan
    };

    struct RenderFrame
    {
        std::uint64_t FrameNumber = 0;

        double DeltaSeconds = 0.0;

        std::uint32_t Width = 1920;
        std::uint32_t Height = 1080;
    };

    class OrbitRenderCore
    {
    public:

        OrbitRenderCore() = default;
        ~OrbitRenderCore();

        bool Initialize(
            const EngineConfig& Config,
            void* NativeWindowHandle
        );

        void BeginFrame(
            const RenderFrame& Frame
        );

        void RenderWorld();

        void RenderRain();

        void RenderReflections();

        void EndFrame();

        void Shutdown();

        bool IsInitialized() const;

        RenderBackend GetBackend() const;

    private:

        bool InitializeDirectX12(
            void* NativeWindowHandle
        );

        bool InitializeVulkan(
            void* NativeWindowHandle
        );

    private:

        RenderBackend Backend =
            RenderBackend::None;

        bool bInitialized = false;

        std::uint64_t FrameCounter = 0;

        std::uint32_t Width = 1920;

        std::uint32_t Height = 1080;
    };
}
