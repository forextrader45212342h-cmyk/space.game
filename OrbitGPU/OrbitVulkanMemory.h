#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>

namespace OrbitGPU {

class VulkanMemory {
public:
    VulkanMemory();

    void SetMemoryProperties(
        const VkPhysicalDeviceMemoryProperties& properties
    );

    uint32_t FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    ) const;

    bool Allocate(
        VkDevice device,
        VkMemoryRequirements requirements,
        VkMemoryPropertyFlags properties,
        VkDeviceMemory& memory
    );

    void Free(
        VkDevice device,
        VkDeviceMemory memory
    );

private:
    VkPhysicalDeviceMemoryProperties memoryProperties_;
    bool initialized_;
};

}
