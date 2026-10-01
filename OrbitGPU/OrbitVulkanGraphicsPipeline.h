#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class OrbitVulkanGraphicsPipeline
{
public:
    OrbitVulkanGraphicsPipeline();
    ~OrbitVulkanGraphicsPipeline();

    bool Create(
        VkDevice device,
        VkPipelineCache cache,
        VkPipelineLayout layout,
        VkRenderPass renderPass,
        VkShaderModule vertexShader,
        VkShaderModule fragmentShader,
        VkExtent2D extent
    );

    void Destroy();

    VkPipeline Get() const;
    bool IsValid() const;

private:
    VkDevice device_;
    VkPipeline pipeline_;
};
