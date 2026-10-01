#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanDescriptorSet
{
public:
    OrbitVulkanDescriptorSet();

    bool Allocate(
        VkDevice device,
        VkDescriptorPool pool,
        VkDescriptorSetLayout layout
    );

    void Reset();

    VkDescriptorSet Get() const;
    bool IsValid() const;

private:
    VkDescriptorSet descriptorSet_;
};
