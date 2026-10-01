#pragma once

#include "OrbitGPUTypes.h"

#include <vector>
#include <cstdint>

namespace OrbitGPU
{
    class GPUTexture
    {
    private:
        TextureDesc desc{};
        std::vector<uint8_t> pixels;
        bool created = false;

        size_t BytesPerPixel() const;

    public:
        bool Create(
            const TextureDesc& description,
            const void* data = nullptr,
            size_t dataSize = 0);

        bool Upload(
            const void* data,
            size_t dataSize);

        void Destroy();

        bool IsValid() const;

        const TextureDesc& GetDescription() const;

        const std::vector<uint8_t>& GetPixels() const;
    };
}
