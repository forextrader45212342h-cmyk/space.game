#include "OrbitVulkanBuffer.h"

#include <cstring>

namespace OrbitGPU {

VulkanBuffer::VulkanBuffer()
    : device_(VK_NULL_HANDLE),
      physicalDevice_(VK_NULL_HANDLE),
      buffer_(VK_NULL_HANDLE),
      memory_(VK_NULL_HANDLE),
      size_(0),
      initialized_(false) {
}

bool VulkanBuffer::Initialize(
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags memoryProperties) {

    if (device == VK_NULL_HANDLE ||
        physicalDevice == VK_NULL_HANDLE ||
        size == 0) {
        return false;
    }

    device_ = device;
    physicalDevice_ = physicalDevice;
    size_ = size;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType =
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode =
        VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(
            device_,
            &bufferInfo,
            nullptr,
            &buffer_) != VK_SUCCESS) {

        buffer_ = VK_NULL_HANDLE;
        return false;
    }

    VkMemoryRequirements requirements{};

    vkGetBufferMemoryRequirements(
        device_,
        buffer_,
        &requirements
    );

    const uint32_t memoryType =
        FindMemoryType(
            requirements.memoryTypeBits,
            memoryProperties
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

    if (vkBindBufferMemory(
            device_,
            buffer_,
            memory_,
            0) != VK_SUCCESS) {

        Shutdown();
        return false;
    }

    initialized_ = true;

    return true;
}

void VulkanBuffer::Shutdown() {
    if (device_ != VK_NULL_HANDLE) {

        if (buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(
                device_,
                buffer_,
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

    buffer_ = VK_NULL_HANDLE;
    memory_ = VK_NULL_HANDLE;

    device_ = VK_NULL_HANDLE;
    physicalDevice_ = VK_NULL_HANDLE;

    size_ = 0;
    initialized_ = false;
}

bool VulkanBuffer::Upload(
    const void* data,
    VkDeviceSize size,
    VkDeviceSize offset) {

    if (!initialized_ ||
        data == nullptr ||
        memory_ == VK_NULL_HANDLE) {
        return false;
    }

    if (offset + size > size_) {
        return false;
    }

    void* mapped = nullptr;

    if (vkMapMemory(
            device_,
            memory_,
            offset,
            size,
            0,
            &mapped) != VK_SUCCESS) {

        return false;
    }

    std::memcpy(
        mapped,
        data,
        static_cast<size_t>(size)
    );

    vkUnmapMemory(
        device_,
        memory_
    );

    return true;
}

VkBuffer VulkanBuffer::Get() const {
    return buffer_;
}

VkDeviceMemory VulkanBuffer::GetMemory() const {
    return memory_;
}

VkDeviceSize VulkanBuffer::GetSize() const {
    return size_;
}

bool VulkanBuffer::IsValid() const {
    return initialized_ &&
           buffer_ != VK_NULL_HANDLE &&
           memory_ != VK_NULL_HANDLE;
}

uint32_t VulkanBuffer::FindMemoryType(
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
