#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanCommandRecorder
{
public:
    static bool Begin(
        VkCommandBuffer commandBuffer
    );

    static bool BeginRenderPass(
        VkCommandBuffer commandBuffer,
        VkRenderPass renderPass,
        VkFramebuffer framebuffer,
        VkExtent2D extent
    );

    static bool EndRenderPass(
        VkCommandBuffer commandBuffer
    );

    static bool End(
        VkCommandBuffer commandBuffer
    );
};
