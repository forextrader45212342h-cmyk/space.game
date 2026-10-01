#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanDepthBuffer
{
public:
    OrbitVulkanDepthBuffer();
    ~OrbitVulkanDepthBuffer();

    bool Create(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        uint32_t width,
        uint32_t height
    );

    void Destroy();

    VkImage GetImage() const;
    VkImageView GetImageView() const;
    VkDeviceMemory GetMemory() const;
    VkFormat GetFormat() const;

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
    VkImage image_;
    VkImageView imageView_;
    VkDeviceMemory memory_;
    VkFormat format_;
    uint32_t width_;
    uint32_t height_;
};
