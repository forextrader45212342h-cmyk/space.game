#include "OrbitVulkanCommandPool.h"

namespace OrbitGPU {

VulkanCommandPool::VulkanCommandPool()
    : device_(VK_NULL_HANDLE),
      commandPool_(VK_NULL_HANDLE),
      initialized_(false) {
}

bool VulkanCommandPool::Initialize(
    VkDevice device,
    uint32_t queueFamilyIndex) {

    if (device == VK_NULL_HANDLE) {
        return false;
    }

    device_ = device;

    VkCommandPoolCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    info.flags =
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    info.queueFamilyIndex = queueFamilyIndex;

    VkResult result = vkCreateCommandPool(
        device_,
        &info,
        nullptr,
        &commandPool_
    );

    if (result != VK_SUCCESS) {
        commandPool_ = VK_NULL_HANDLE;
        return false;
    }

    initialized_ = true;

    return true;
}

void VulkanCommandPool::Shutdown() {
    if (commandPool_ != VK_NULL_HANDLE &&
        device_ != VK_NULL_HANDLE) {

        vkDestroyCommandPool(
            device_,
            commandPool_,
            nullptr
        );
    }

    commandPool_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
    initialized_ = false;
}

VkCommandPool VulkanCommandPool::Get() const {
    return commandPool_;
}

bool VulkanCommandPool::IsValid() const {
    return initialized_ &&
           commandPool_ != VK_NULL_HANDLE;
}

}
