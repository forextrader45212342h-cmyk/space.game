#include "OrbitVulkanImage.h"

namespace OrbitGPU {

VulkanImage::VulkanImage()
    : device_(VK_NULL_HANDLE),
      physicalDevice_(VK_NULL_HANDLE),
      image_(VK_NULL_HANDLE),
      memory_(VK_NULL_HANDLE),
      width_(0),
      height_(0),
      format_(VK_FORMAT_UNDEFINED),
      aspect_(0),
      initialized_(false) {
}

bool VulkanImage::Initialize(
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    uint32_t width,
    uint32_t height,
    VkFormat format,
    VkImageUsageFlags usage,
    VkImageAspectFlags aspect) {

    if (device == VK_NULL_HANDLE ||
        physicalDevice == VK_NULL_HANDLE ||
        width == 0 ||
        height == 0) {
        return false;
    }

    device_ = device;
    physicalDevice_ = physicalDevice;
    width_ = width;
    height_ = height;
    format_ = format;
    aspect_ = aspect;

    VkImageCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

    info.imageType =
        VK_IMAGE_TYPE_2D;

    info.extent.width = width;
    info.extent.height = height;
    info.extent.depth = 1;

    info.mipLevels = 1;
    info.arrayLayers = 1;

    info.format = format;

    info.tiling =
        VK_IMAGE_TILING_OPTIMAL;

    info.initialLayout =
        VK_IMAGE_LAYOUT_UNDEFINED;

    info.usage = usage;

    info.samples =
        VK_SAMPLE_COUNT_1_BIT;

    info.sharingMode =
        VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(
            device_,
            &info,
            nullptr,
            &image_) != VK_SUCCESS) {

        image_ = VK_NULL_HANDLE;
        return false;
    }

    VkMemoryRequirements requirements{};

    vkGetImageMemoryRequirements(
        device_,
        image_,
        &requirements
    );

    const uint32_t memoryType =
        FindMemoryType(
            requirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

    if (memoryType == UINT32_MAX) {
        Shutdown();
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
            device_,
            &allocation,
            nullptr,
            &memory_) != VK_SUCCESS) {

        Shutdown();
        return false;
    }

    if (vkBindImageMemory(
            device_,
            image_,
            memory_,
            0) != VK_SUCCESS) {

        Shutdown();
        return false;
    }

    initialized_ = true;

    return true;
}

void VulkanImage::Shutdown() {
    if (device_ != VK_NULL_HANDLE) {

        if (image_ != VK_NULL_HANDLE) {
            vkDestroyImage(
                device_,
                image_,
                nullptr
            );
        }

        if (memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(
                device_,
                memory_,
                nullptr
            );
        }
    }

    image_ = VK_NULL_HANDLE;
    memory_ = VK_NULL_HANDLE;

    device_ = VK_NULL_HANDLE;
    physicalDevice_ = VK_NULL_HANDLE;

    width_ = 0;
    height_ = 0;

    format_ = VK_FORMAT_UNDEFINED;
    aspect_ = 0;

    initialized_ = false;
}

VkImage VulkanImage::Get() const {
    return image_;
}

VkDeviceMemory VulkanImage::GetMemory() const {
    return memory_;
}

uint32_t VulkanImage::GetWidth() const {
    return width_;
}

uint32_t VulkanImage::GetHeight() const {
    return height_;
}

bool VulkanImage::IsValid() const {
    return initialized_ &&
           image_ != VK_NULL_HANDLE &&
           memory_ != VK_NULL_HANDLE;
}

uint32_t VulkanImage::FindMemoryType(
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties) const {

    VkPhysicalDeviceMemoryProperties memoryProperties{};

    vkGetPhysicalDeviceMemoryProperties(
        physicalDevice_,
        &memoryProperties
    );

    for (uint32_t i = 0;
         i < memoryProperties.memoryTypeCount;
         ++i) {

        const bool typeSupported =
            (typeFilter & (1u << i)) != 0;

        const bool propertiesSupported =
            (memoryProperties.memoryTypes[i].propertyFlags &
             properties) == properties;

        if (typeSupported &&
            propertiesSupported) {

            return i;
        }
    }

    return UINT32_MAX;
}

}
