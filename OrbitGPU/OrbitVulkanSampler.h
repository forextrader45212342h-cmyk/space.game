#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanSampler
{
public:
    OrbitVulkanSampler();
    ~OrbitVulkanSampler();

    bool Create(
        VkDevice device,
        VkFilter filter = VK_FILTER_LINEAR,
        VkSamplerAddressMode addressMode =
            VK_SAMPLER_ADDRESS_MODE_REPEAT
    );

    void Destroy();

    VkSampler Get() const;
    bool IsValid() const;

private:
    VkDevice device_;
    VkSampler sampler_;
};
