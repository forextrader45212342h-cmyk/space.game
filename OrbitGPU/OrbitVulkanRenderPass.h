#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanRenderPass
{
public:
    OrbitVulkanRenderPass();
    ~OrbitVulkanRenderPass();

    bool Create(
        VkDevice device,
        VkFormat colorFormat,
        VkFormat depthFormat
    );

    void Destroy();

    VkRenderPass Get() const;
    bool IsValid() const;

private:
    VkDevice device_;
    VkRenderPass renderPass_;
};
