#include "OrbitGPUTexture.h"

#include <cstring>

namespace OrbitGPU
{
    size_t GPUTexture::BytesPerPixel() const
    {
        switch (desc.format)
        {
            case PixelFormat::RGBA8:
            case PixelFormat::BGRA8:
                return 4;

            case PixelFormat::RGBA16F:
                return 8;

            case PixelFormat::RGBA32F:
                return 16;

            case PixelFormat::Depth32F:
                return 4;
        }

        return 0;
    }

    bool GPUTexture::Create(
        const TextureDesc& description,
        const void* data,
        size_t dataSize)
    {
        if (description.width == 0 ||
            description.height == 0)
            return false;

        desc = description;

        const size_t required =
            static_cast<size_t>(desc.width) *
            static_cast<size_t>(desc.height) *
            static_cast<size_t>(desc.depth) *
            BytesPerPixel();

        if (required == 0)
            return false;

        try
        {
            pixels.resize(required);
        }
        catch (...)
        {
            pixels.clear();
            return false;
        }

        if (data)
        {
            if (dataSize != required)
            {
                Destroy();
                return false;
            }

            std::memcpy(
                pixels.data(),
                data,
                required);
        }

        created = true;
        return true;
    }

    bool GPUTexture::Upload(
        const void* data,
        size_t dataSize)
    {
        if (!created || !data)
            return false;

        if (dataSize != pixels.size())
            return false;

        std::memcpy(
            pixels.data(),
            data,
            dataSize);

        return true;
    }

    void GPUTexture::Destroy()
    {
        pixels.clear();
        pixels.shrink_to_fit();
        desc = {};
        created = false;
    }

    bool GPUTexture::IsValid() const
    {
        return created && !pixels.empty();
    }

    const TextureDesc& GPUTexture::GetDescription() const
    {
        return desc;
    }

    const std::vector<uint8_t>& GPUTexture::GetPixels() const
    {
        return pixels;
    }
}
