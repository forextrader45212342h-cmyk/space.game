#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>
#include <cstddef>

namespace OrbitGPU {

class VulkanBuffer {
public:
    VulkanBuffer();

    bool Initialize(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags memoryProperties
    );

    void Shutdown();

    bool Upload(
        const void* data,
        VkDeviceSize size,
        VkDeviceSize offset = 0
    );

    VkBuffer Get() const;
    VkDeviceMemory GetMemory() const;
    VkDeviceSize GetSize() const;

    bool IsValid() const;

private:
    VkDevice device_;
    VkPhysicalDevice physicalDevice_;

    VkBuffer buffer_;
    VkDeviceMemory memory_;

    VkDeviceSize size_;

    bool initialized_;

    uint32_t FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    ) const;
};

}
