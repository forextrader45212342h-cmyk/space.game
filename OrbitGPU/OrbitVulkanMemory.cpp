#include "OrbitVulkanMemory.h"

namespace OrbitGPU {

VulkanMemory::VulkanMemory()
    : memoryProperties_{},
      initialized_(false) {
}

void VulkanMemory::SetMemoryProperties(
    const VkPhysicalDeviceMemoryProperties& properties) {

    memoryProperties_ = properties;
    initialized_ = true;
}

uint32_t VulkanMemory::FindMemoryType(
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties) const {

    if (!initialized_) {
        return UINT32_MAX;
    }

    for (uint32_t i = 0;
         i < memoryProperties_.memoryTypeCount;
         ++i) {

        const bool typeSupported =
            (typeFilter & (1u << i)) != 0;

        const bool flagsSupported =
            (memoryProperties_.memoryTypes[i].propertyFlags &
             properties) == properties;

        if (typeSupported && flagsSupported) {
            return i;
        }
    }

    return UINT32_MAX;
}

bool VulkanMemory::Allocate(
    VkDevice device,
    VkMemoryRequirements requirements,
    VkMemoryPropertyFlags properties,
    VkDeviceMemory& memory) {

    memory = VK_NULL_HANDLE;

    if (!initialized_ ||
        device == VK_NULL_HANDLE) {
        return false;
    }

    const uint32_t memoryType =
        FindMemoryType(
            requirements.memoryTypeBits,
            properties
        );

    if (memoryType == UINT32_MAX) {
        return false;
    }

    VkMemoryAllocateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    info.allocationSize =
        requirements.size;

    info.memoryTypeIndex =
        memoryType;

    return vkAllocateMemory(
        device,
        &info,
        nullptr,
        &memory
    ) == VK_SUCCESS;
}

void VulkanMemory::Free(
    VkDevice device,
    VkDeviceMemory memory) {

    if (device != VK_NULL_HANDLE &&
        memory != VK_NULL_HANDLE) {

        vkFreeMemory(
            device,
            memory,
            nullptr
        );
    }
}

}
