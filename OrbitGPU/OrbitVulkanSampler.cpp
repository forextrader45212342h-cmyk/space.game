#include "OrbitVulkanSampler.h"

OrbitVulkanSampler::OrbitVulkanSampler()
    : device_(VK_NULL_HANDLE),
      sampler_(VK_NULL_HANDLE)
{
}

OrbitVulkanSampler::~OrbitVulkanSampler()
{
    Destroy();
}

bool OrbitVulkanSampler::Create(
    VkDevice device,
    VkFilter filter,
    VkSamplerAddressMode addressMode)
{
    if (device == VK_NULL_HANDLE)
        return false;

    Destroy();

    VkSamplerCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

    info.magFilter = filter;
    info.minFilter = filter;

    info.mipmapMode =
        VK_SAMPLER_MIPMAP_MODE_LINEAR;

    info.addressModeU = addressMode;
    info.addressModeV = addressMode;
    info.addressModeW = addressMode;

    info.mipLodBias = 0.0f;
    info.anisotropyEnable = VK_FALSE;
    info.maxAnisotropy = 1.0f;

    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_ALWAYS;

    info.minLod = 0.0f;
    info.maxLod = VK_LOD_CLAMP_NONE;

    info.borderColor =
        VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;

    info.unnormalizedCoordinates = VK_FALSE;

    if (vkCreateSampler(
            device,
            &info,
            nullptr,
            &sampler_) != VK_SUCCESS)
    {
        sampler_ = VK_NULL_HANDLE;
        return false;
    }

    device_ = device;
    return true;
}

void OrbitVulkanSampler::Destroy()
{
    if (device_ != VK_NULL_HANDLE &&
        sampler_ != VK_NULL_HANDLE)
    {
        vkDestroySampler(
            device_,
            sampler_,
            nullptr
        );
    }

    sampler_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkSampler OrbitVulkanSampler::Get() const
{
    return sampler_;
}

bool OrbitVulkanSampler::IsValid() const
{
    return sampler_ != VK_NULL_HANDLE;
}
