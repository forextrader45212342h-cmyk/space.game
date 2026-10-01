#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class OrbitVulkanDescriptorPool
{
public:
    OrbitVulkanDescriptorPool();
    ~OrbitVulkanDescriptorPool();

    bool Create(
        VkDevice device,
        const std::vector<VkDescriptorPoolSize>& sizes,
        uint32_t maxSets
    );

    void Destroy();

    VkDescriptorPool Get() const;
    bool IsValid() const;

private:
    VkDevice device_;
    VkDescriptorPool pool_;
};
