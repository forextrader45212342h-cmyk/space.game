#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanCommandSubmitter
{
public:
    static bool Submit(
        VkQueue queue,
        VkCommandBuffer commandBuffer,
        VkSemaphore waitSemaphore,
        VkSemaphore signalSemaphore,
        VkFence fence
    );

    static bool Present(
        VkQueue queue,
        VkSwapchainKHR swapchain,
        uint32_t imageIndex,
        VkSemaphore waitSemaphore
    );
};
