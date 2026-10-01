#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class OrbitVulkanDescriptorSetLayout
{
public:
    OrbitVulkanDescriptorSetLayout();
    ~OrbitVulkanDescriptorSetLayout();

    bool Create(
        VkDevice device,
        const std::vector<VkDescriptorSetLayoutBinding>& bindings
    );

    void Destroy();

    VkDescriptorSetLayout Get() const;
    bool IsValid() const;

private:
    VkDevice device_;
    VkDescriptorSetLayout layout_;
};
