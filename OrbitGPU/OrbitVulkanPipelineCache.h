#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanPipelineCache
{
public:
    OrbitVulkanPipelineCache();
    ~OrbitVulkanPipelineCache();

    bool Create(VkDevice device);
    void Destroy();

    VkPipelineCache Get() const;
    bool IsValid() const;

private:
    VkDevice device_;
    VkPipelineCache cache_;
};
