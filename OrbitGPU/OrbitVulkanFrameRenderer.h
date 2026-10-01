#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanFrameRenderer
{
public:
    static VkResult Acquire(
        VkDevice device,
        VkSwapchainKHR swapchain,
        VkSemaphore imageAvailable,
        uint32_t& imageIndex
    );

    static bool DrawTriangle(
        VkCommandBuffer commandBuffer,
        VkPipeline pipeline,
        VkPipelineLayout layout,
        VkExtent2D extent
    );
};

