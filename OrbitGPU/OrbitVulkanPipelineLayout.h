#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class OrbitVulkanPipelineLayout
{
public:
    OrbitVulkanPipelineLayout();
    ~OrbitVulkanPipelineLayout();

    bool Create(
        VkDevice device,
        const std::vector<VkDescriptorSetLayout>& layouts
    );

    void Destroy();

    VkPipelineLayout Get() const;
    bool IsValid() const;

private:
    VkDevice device_;
    VkPipelineLayout pipelineLayout_;
};
