#include "OrbitVulkanSwapchainImageViews.h"

OrbitVulkanSwapchainImageViews::
OrbitVulkanSwapchainImageViews()
    : device_(VK_NULL_HANDLE)
{
}

OrbitVulkanSwapchainImageViews::
~OrbitVulkanSwapchainImageViews()
{
    Destroy();
}

bool OrbitVulkanSwapchainImageViews::Create(
    VkDevice device,
    const std::vector<VkImage>& images,
    VkFormat format)
{
    if (device == VK_NULL_HANDLE ||
        images.empty())
    {
        return false;
    }

    Destroy();

    views_.resize(images.size());

    for (size_t i = 0; i < images.size(); ++i)
    {
        VkImageViewCreateInfo info{};

        info.sType =
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

        info.image = images[i];

        info.viewType =
            VK_IMAGE_VIEW_TYPE_2D;

        info.format = format;

        info.components.r =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        info.components.g =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        info.components.b =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        info.components.a =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        info.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;

        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;

        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(
                device,
                &info,
                nullptr,
                &views_[i]) != VK_SUCCESS)
        {
            Destroy();
            return false;
        }
    }

    device_ = device;

    return true;
}

void OrbitVulkanSwapchainImageViews::Destroy()
{
    if (device_ != VK_NULL_HANDLE)
    {
        for (VkImageView view : views_)
        {
            if (view != VK_NULL_HANDLE)
            {
                vkDestroyImageView(
                    device_,
                    view,
                    nullptr
                );
            }
        }
    }

    views_.clear();
    device_ = VK_NULL_HANDLE;
}

const std::vector<VkImageView>&
OrbitVulkanSwapchainImageViews::GetViews() const
{
    return views_;
}

bool OrbitVulkanSwapchainImageViews::IsValid() const
{
    return !views_.empty();
}
