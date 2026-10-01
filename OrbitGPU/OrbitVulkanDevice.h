#pragma once

#include <vulkan/vulkan.h>
#include "OrbitVulkanPhysicalDevice.h"

namespace OrbitGPU {

class VulkanDevice {
public:
    VulkanDevice();

    bool Initialize(
        const VulkanPhysicalDevice& physicalDevice
    );

    void Shutdown();

    VkDevice Get() const;

    VkQueue GetGraphicsQueue() const;
    VkQueue GetComputeQueue() const;
    VkQueue GetTransferQueue() const;

    bool IsValid() const;

private:
    VkDevice device_;

    VkQueue graphicsQueue_;
    VkQueue computeQueue_;
    VkQueue transferQueue_;

    bool initialized_;
};

}
