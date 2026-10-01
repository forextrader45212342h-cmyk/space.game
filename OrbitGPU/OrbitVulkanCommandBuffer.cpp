#include "OrbitVulkanCommandBuffer.h"

namespace OrbitGPU {

VulkanCommandBuffer::VulkanCommandBuffer()
    : device_(VK_NULL_HANDLE),
      commandPool_(VK_NULL_HANDLE),
      commandBuffer_(VK_NULL_HANDLE),
      recording_(false) {
}

bool VulkanCommandBuffer::Allocate(
    VkDevice device,
    VkCommandPool commandPool,
    VkCommandBufferLevel level) {

    if (device == VK_NULL_HANDLE ||
        commandPool == VK_NULL_HANDLE) {
        return false;
    }

    device_ = device;
    commandPool_ = commandPool;

    VkCommandBufferAllocateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

    info.commandPool = commandPool_;
    info.level = level;
    info.commandBufferCount = 1;

    VkResult result = vkAllocateCommandBuffers(
        device_,
        &info,
        &commandBuffer_
    );

    if (result != VK_SUCCESS) {
        commandBuffer_ = VK_NULL_HANDLE;
        return false;
    }

    return true;
}

void VulkanCommandBuffer::Free() {
    if (commandBuffer_ != VK_NULL_HANDLE &&
        device_ != VK_NULL_HANDLE &&
        commandPool_ != VK_NULL_HANDLE) {

        vkFreeCommandBuffers(
            device_,
            commandPool_,
            1,
            &commandBuffer_
        );
    }

    commandBuffer_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
    commandPool_ = VK_NULL_HANDLE;
    recording_ = false;
}

bool VulkanCommandBuffer::Begin(
    VkCommandBufferUsageFlags flags) {

    if (commandBuffer_ == VK_NULL_HANDLE ||
        recording_) {
        return false;
    }

    VkCommandBufferBeginInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    info.flags = flags;

    VkResult result =
        vkBeginCommandBuffer(
            commandBuffer_,
            &info
        );

    if (result != VK_SUCCESS) {
        return false;
    }

    recording_ = true;

    return true;
}

bool VulkanCommandBuffer::End() {
    if (!recording_) {
        return false;
    }

    VkResult result =
        vkEndCommandBuffer(commandBuffer_);

    if (result != VK_SUCCESS) {
        return false;
    }

    recording_ = false;

    return true;
}

void VulkanCommandBuffer::Reset() {
    if (commandBuffer_ != VK_NULL_HANDLE) {
        vkResetCommandBuffer(
            commandBuffer_,
            0
        );
    }

    recording_ = false;
}

VkCommandBuffer VulkanCommandBuffer::Get() const {
    return commandBuffer_;
}

bool VulkanCommandBuffer::IsValid() const {
    return commandBuffer_ != VK_NULL_HANDLE;
}

}
