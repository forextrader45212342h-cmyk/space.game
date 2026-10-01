#pragma once

#include <vulkan/vulkan.h>

namespace OrbitGPU {

class VulkanFence {
public:
    VulkanFence();

    bool Initialize(
        VkDevice device,
        bool signaled = false
    );

    void Shutdown();

    bool Wait(uint64_t timeout = UINT64_MAX);
    bool Reset();

    VkFence Get() const;

private:
    VkDevice device_;
    VkFence fence_;
};

class VulkanSemaphore {
public:
    VulkanSemaphore();

    bool Initialize(VkDevice device);
    void Shutdown();

    VkSemaphore Get() const;

private:
    VkDevice device_;
    VkSemaphore semaphore_;
};

}
