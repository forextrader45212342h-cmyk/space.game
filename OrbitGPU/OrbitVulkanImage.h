#pragma once

#include <vulkan/vulkan.h>

namespace OrbitGPU {

class VulkanImage {
public:
    VulkanImage();

    bool Initialize(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        uint32_t width,
        uint32_t height,
        VkFormat format,
        VkImageUsageFlags usage,
        VkImageAspectFlags aspect
    );

    void Shutdown();

    VkImage Get() const;
    VkDeviceMemory GetMemory() const;

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

    bool IsValid() const;

private:
    VkDevice device_;
    VkPhysicalDevice physicalDevice_;

    VkImage image_;
    VkDeviceMemory memory_;

    uint32_t width_;
    uint32_t height_;

    VkFormat format_;
    VkImageAspectFlags aspect_;

    bool initialized_;

    uint32_t FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    ) const;
};

}
