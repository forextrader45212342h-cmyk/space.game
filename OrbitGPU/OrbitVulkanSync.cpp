#include "OrbitVulkanSync.h"

namespace OrbitGPU {

VulkanFence::VulkanFence()
    : device_(VK_NULL_HANDLE),
      fence_(VK_NULL_HANDLE) {
}

bool VulkanFence::Initialize(
    VkDevice device,
    bool signaled) {

    if (device == VK_NULL_HANDLE) {
        return false;
    }

    device_ = device;

    VkFenceCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    if (signaled) {
        info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    VkResult result = vkCreateFence(
        device_,
        &info,
        nullptr,
        &fence_
    );

    return result == VK_SUCCESS;
}

void VulkanFence::Shutdown() {
    if (fence_ != VK_NULL_HANDLE &&
        device_ != VK_NULL_HANDLE) {

        vkDestroyFence(
            device_,
            fence_,
            nullptr
        );
    }

    fence_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

bool VulkanFence::Wait(uint64_t timeout) {
    if (fence_ == VK_NULL_HANDLE) {
        return false;
    }

    return vkWaitForFences(
        device_,
        1,
        &fence_,
        VK_TRUE,
        timeout
    ) == VK_SUCCESS;
}

bool VulkanFence::Reset() {
    if (fence_ == VK_NULL_HANDLE) {
        return false;
    }

    return vkResetFences(
        device_,
        1,
        &fence_
    ) == VK_SUCCESS;
}

VkFence VulkanFence::Get() const {
    return fence_;
}

VulkanSemaphore::VulkanSemaphore()
    : device_(VK_NULL_HANDLE),
      semaphore_(VK_NULL_HANDLE) {
}

bool VulkanSemaphore::Initialize(VkDevice device) {
    if (device == VK_NULL_HANDLE) {
        return false;
    }

    device_ = device;

    VkSemaphoreCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkResult result = vkCreateSemaphore(
        device_,
        &info,
        nullptr,
        &semaphore_
    );

    return result == VK_SUCCESS;
}

void VulkanSemaphore::Shutdown() {
    if (semaphore_ != VK_NULL_HANDLE &&
        device_ != VK_NULL_HANDLE) {

        vkDestroySemaphore(
            device_,
            semaphore_,
            nullptr
        );
    }

    semaphore_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkSemaphore VulkanSemaphore::Get() const {
    return semaphore_;
}

}
