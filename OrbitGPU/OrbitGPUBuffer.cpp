#include "OrbitGPUBuffer.h"

#include <cstring>

namespace OrbitGPU
{
    bool GPUBuffer::Create(const BufferDesc& description)
    {
        if (description.size == 0)
            return false;

        desc = description;

        try
        {
            storage.resize(desc.size);
        }
        catch (...)
        {
            storage.clear();
            return false;
        }

        created = true;
        return true;
    }

    bool GPUBuffer::Upload(
        const void* data,
        size_t size,
        size_t offset)
    {
        if (!created || !data)
            return false;

        if (offset > storage.size())
            return false;

        if (size > storage.size() - offset)
            return false;

        std::memcpy(
            storage.data() + offset,
            data,
            size);

        return true;
    }

    void GPUBuffer::Destroy()
    {
        storage.clear();
        storage.shrink_to_fit();
        desc = {};
        created = false;
    }

    bool GPUBuffer::IsValid() const
    {
        return created && !storage.empty();
    }

    const BufferDesc& GPUBuffer::GetDescription() const
    {
        return desc;
    }

    const std::vector<uint8_t>& GPUBuffer::GetCPUStorage() const
    {
        return storage;
    }
}
