#include "OrbitVulkanDepthBuffer.h"

OrbitVulkanDepthBuffer::OrbitVulkanDepthBuffer()
    : device_(VK_NULL_HANDLE),
      image_(VK_NULL_HANDLE),
      imageView_(VK_NULL_HANDLE),
      memory_(VK_NULL_HANDLE),
      format_(VK_FORMAT_D32_SFLOAT),
      width_(0),
      height_(0)
{
}

OrbitVulkanDepthBuffer::~OrbitVulkanDepthBuffer()
{
    Destroy();
}

bool OrbitVulkanDepthBuffer::FindMemoryType(
    VkPhysicalDevice physicalDevice,
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties,
    uint32_t& typeIndex)
{
    VkPhysicalDeviceMemoryProperties memoryProperties{};
    vkGetPhysicalDeviceMemoryProperties(
        physicalDevice,
        &memoryProperties
    );

    for (uint32_t i = 0;
         i < memoryProperties.memoryTypeCount;
         ++i)
    {
        if ((typeFilter & (1u << i)) &&
            (memoryProperties.memoryTypes[i].propertyFlags &
             properties) == properties)
        {
            typeIndex = i;
            return true;
        }
    }

    return false;
}

bool OrbitVulkanDepthBuffer::Create(
    VkPhysicalDevice physicalDevice,
    VkDevice device,
    uint32_t width,
    uint32_t height)
{
    if (physicalDevice == VK_NULL_HANDLE ||
        device == VK_NULL_HANDLE ||
        width == 0 ||
        height == 0)
    {
        return false;
    }

    Destroy();

    format_ = VK_FORMAT_D32_SFLOAT;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format_;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage =
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(
            device,
            &imageInfo,
            nullptr,
            &image_) != VK_SUCCESS)
    {
        image_ = VK_NULL_HANDLE;
        return false;
    }

    VkMemoryRequirements requirements{};
    vkGetImageMemoryRequirements(
        device,
        image_,
        &requirements
    );

    uint32_t memoryType = 0;

    if (!FindMemoryType(
            physicalDevice,
            requirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            memoryType))
    {
        Destroy();
        return false;
    }

    VkMemoryAllocateInfo allocation{};
    allocation.sType =
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocation.allocationSize =
        requirements.size;
    allocation.memoryTypeIndex =
        memoryType;

    if (vkAllocateMemory(
            device,
            &allocation,
            nullptr,
            &memory_) != VK_SUCCESS)
    {
        Destroy();
        return false;
    }

    if (vkBindImageMemory(
            device,
            image_,
            memory_,
            0) != VK_SUCCESS)
    {
        Destroy();
        return false;
    }

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType =
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image_;
    viewInfo.viewType =
        VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format_;

    viewInfo.subresourceRange.aspectMask =
        VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(
            device,
            &viewInfo,
            nullptr,
            &imageView_) != VK_SUCCESS)
    {
        Destroy();
        return false;
    }

    device_ = device;
    width_ = width;
    height_ = height;

    return true;
}

void OrbitVulkanDepthBuffer::Destroy()
{
    if (device_ != VK_NULL_HANDLE)
    {
        if (imageView_ != VK_NULL_HANDLE)
        {
            vkDestroyImageView(
                device_,
                imageView_,
                nullptr
            );
        }

        if (image_ != VK_NULL_HANDLE)
        {
            vkDestroyImage(
                device_,
                image_,
                nullptr
            );
        }

        if (memory_ != VK_NULL_HANDLE)
        {
            vkFreeMemory(
                device_,
                memory_,
                nullptr
            );
        }
    }

    device_ = VK_NULL_HANDLE;
    image_ = VK_NULL_HANDLE;
    imageView_ = VK_NULL_HANDLE;
    memory_ = VK_NULL_HANDLE;
    width_ = 0;
    height_ = 0;
}

VkImage OrbitVulkanDepthBuffer::GetImage() const
{
    return image_;
}

VkImageView OrbitVulkanDepthBuffer::GetImageView() const
{
    return imageView_;
}

VkDeviceMemory OrbitVulkanDepthBuffer::GetMemory() const
{
    return memory_;
}

VkFormat OrbitVulkanDepthBuffer::GetFormat() const
{
    return format_;
}

bool OrbitVulkanDepthBuffer::IsValid() const
{
    return image_ != VK_NULL_HANDLE &&
           imageView_ != VK_NULL_HANDLE &&
           memory_ != VK_NULL_HANDLE;
}
