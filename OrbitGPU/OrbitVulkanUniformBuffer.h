#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanUniformBuffer
{
public:
    OrbitVulkanUniformBuffer();
    ~OrbitVulkanUniformBuffer();

    bool Create(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        VkDeviceSize size
    );

    bool Upload(
        const void* data,
        VkDeviceSize size,
        VkDeviceSize offset = 0
    );

    void Destroy();

    VkBuffer GetBuffer() const;
    VkDeviceMemory GetMemory() const;
    VkDeviceSize GetSize() const;

    bool IsValid() const;

private:
    bool FindMemoryType(
        VkPhysicalDevice physicalDevice,
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties,
        uint32_t& typeIndex
    );

private:
    VkDevice device_;
    VkBuffer buffer_;
    VkDeviceMemory memory_;
    VkDeviceSize size_;
};
