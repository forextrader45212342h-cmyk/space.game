#include "OrbitVulkanImageView.h"

OrbitVulkanImageView::OrbitVulkanImageView()
    : device_(VK_NULL_HANDLE),
      imageView_(VK_NULL_HANDLE)
{
}

OrbitVulkanImageView::~OrbitVulkanImageView()
{
    Destroy();
}

bool OrbitVulkanImageView::Create(
    VkDevice device,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspectFlags,
    VkImageViewType viewType,
    uint32_t mipLevels,
    uint32_t layers)
{
    if (device == VK_NULL_HANDLE || image == VK_NULL_HANDLE)
        return false;

    Destroy();

    VkImageViewCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image;
    info.viewType = viewType;
    info.format = format;

    info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    info.subresourceRange.aspectMask = aspectFlags;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = mipLevels;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = layers;

    if (vkCreateImageView(device, &info, nullptr, &imageView_) != VK_SUCCESS)
    {
        imageView_ = VK_NULL_HANDLE;
        return false;
    }

    device_ = device;
    return true;
}

void OrbitVulkanImageView::Destroy()
{
    if (device_ != VK_NULL_HANDLE &&
        imageView_ != VK_NULL_HANDLE)
    {
        vkDestroyImageView(device_, imageView_, nullptr);
    }

    imageView_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkImageView OrbitVulkanImageView::Get() const
{
    return imageView_;
}

bool OrbitVulkanImageView::IsValid() const
{
    return imageView_ != VK_NULL_HANDLE;
}
