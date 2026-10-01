#pragma once

#include <vulkan/vulkan.h>

class OrbitVulkanImageView
{
public:
    OrbitVulkanImageView();
    ~OrbitVulkanImageView();

    bool Create(
        VkDevice device,
        VkImage image,
        VkFormat format,
        VkImageAspectFlags aspectFlags,
        VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D,
        uint32_t mipLevels = 1,
        uint32_t layers = 1
    );

    void Destroy();

    VkImageView Get() const;
    bool IsValid() const;

private:
    VkDevice device_;
    VkImageView imageView_;
};
