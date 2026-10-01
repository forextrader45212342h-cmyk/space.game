#pragma once

#include <vulkan/vulkan.h>

namespace OrbitGPU {

class VulkanCommandBuffer {
public:
    VulkanCommandBuffer();

    bool Allocate(
        VkDevice device,
        VkCommandPool commandPool,
        VkCommandBufferLevel level =
            VK_COMMAND_BUFFER_LEVEL_PRIMARY
    );

    void Free();

    bool Begin(
        VkCommandBufferUsageFlags flags = 0
    );

    bool End();

    void Reset();

    VkCommandBuffer Get() const;

    bool IsValid() const;

private:
    VkDevice device_;
    VkCommandPool commandPool_;
    VkCommandBuffer commandBuffer_;
    bool recording_;
};

}
