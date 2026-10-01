#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

namespace OrbitGPU {

class VulkanPhysicalDevice {
public:
    VulkanPhysicalDevice();

    bool Select(VkInstance instance);

    VkPhysicalDevice Get() const;

    const VkPhysicalDeviceProperties& GetProperties() const;
    const VkPhysicalDeviceFeatures& GetFeatures() const;
    const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const;

    uint32_t GetGraphicsQueueFamily() const;
    uint32_t GetComputeQueueFamily() const;
    uint32_t GetTransferQueueFamily() const;

    bool IsValid() const;

    std::string GetDeviceName() const;

private:
    VkPhysicalDevice physicalDevice_;

    VkPhysicalDeviceProperties properties_;
    VkPhysicalDeviceFeatures features_;
    VkPhysicalDeviceMemoryProperties memoryProperties_;

    uint32_t graphicsQueueFamily_;
    uint32_t computeQueueFamily_;
    uint32_t transferQueueFamily_;

    bool initialized_;

    bool FindQueueFamilies(VkPhysicalDevice device);
};

}
