#pragma once

#include <vulkan/vulkan.h>

namespace OrbitGPU {

class VulkanCommandPool {
public:
    VulkanCommandPool();

    bool Initialize(
        VkDevice device,
        uint32_t queueFamilyIndex
    );

    void Shutdown();

    VkCommandPool Get() const;

    bool IsValid() const;

private:
    VkDevice device_;
    VkCommandPool commandPool_;
    bool initialized_;
};

}
