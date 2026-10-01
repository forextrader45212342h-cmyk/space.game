#pragma once

#include "OrbitGPUTypes.h"
#include <vector>
#include <cstdint>

namespace OrbitGPU
{
    class GPUBuffer
    {
    private:
        BufferDesc desc{};
        std::vector<uint8_t> storage;
        bool created = false;

    public:
        bool Create(const BufferDesc& description);

        bool Upload(
            const void* data,
            size_t size,
            size_t offset = 0);

        void Destroy();

        bool IsValid() const;

        const BufferDesc& GetDescription() const;

        const std::vector<uint8_t>& GetCPUStorage() const;
    };
}
