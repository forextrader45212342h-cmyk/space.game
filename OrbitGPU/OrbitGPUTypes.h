#pragma once

#include <cstdint>
#include <cstddef>

namespace OrbitGPU
{
    enum class BufferType
    {
        Vertex,
        Index,
        Uniform,
        Storage,
        Indirect
    };

    enum class PixelFormat
    {
        RGBA8,
        BGRA8,
        RGBA16F,
        RGBA32F,
        Depth32F
    };

    struct BufferDesc
    {
        size_t size = 0;
        BufferType type = BufferType::Vertex;
        bool cpuVisible = false;
    };

    struct TextureDesc
    {
        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t depth = 1;
        uint32_t mipLevels = 1;
        PixelFormat format = PixelFormat::RGBA8;
        bool sampled = true;
        bool renderTarget = false;
        bool depthTarget = false;
    };

    struct Viewport
    {
        float x = 0;
        float y = 0;
        float width = 1;
        float height = 1;
        float minDepth = 0;
        float maxDepth = 1;
    };

    struct Scissor
    {
        int32_t x = 0;
        int32_t y = 0;
        uint32_t width = 1;
        uint32_t height = 1;
    };
}
