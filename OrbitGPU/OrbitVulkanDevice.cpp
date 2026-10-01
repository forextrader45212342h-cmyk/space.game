#include "OrbitVulkanDevice.h"

#include <vector>
#include <set>

namespace OrbitGPU {

VulkanDevice::VulkanDevice()
    : device_(VK_NULL_HANDLE),
      graphicsQueue_(VK_NULL_HANDLE),
      computeQueue_(VK_NULL_HANDLE),
      transferQueue_(VK_NULL_HANDLE),
      initialized_(false) {
}

bool VulkanDevice::Initialize(
    const VulkanPhysicalDevice& physicalDevice) {

    if (!physicalDevice.IsValid()) {
        return false;
    }

    const uint32_t graphicsFamily =
        physicalDevice.GetGraphicsQueueFamily();

    const uint32_t computeFamily =
        physicalDevice.GetComputeQueueFamily();

    const uint32_t transferFamily =
        physicalDevice.GetTransferQueueFamily();

    std::set<uint32_t> uniqueFamilies = {
        graphicsFamily,
        computeFamily,
        transferFamily
    };

    const float priority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueInfos;

    for (uint32_t family : uniqueFamilies) {
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

        queueInfo.queueFamilyIndex = family;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &priority;

        queueInfos.push_back(queueInfo);
    }

    VkPhysicalDeviceFeatures features{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueInfos.size());

    createInfo.pQueueCreateInfos =
        queueInfos.data();

    createInfo.pEnabledFeatures =
        &features;

    VkResult result = vkCreateDevice(
        physicalDevice.Get(),
        &createInfo,
        nullptr,
        &device_
    );

    if (result != VK_SUCCESS) {
        device_ = VK_NULL_HANDLE;
        return false;
    }

    vkGetDeviceQueue(
        device_,
        graphicsFamily,
        0,
        &graphicsQueue_
    );

    vkGetDeviceQueue(
        device_,
        computeFamily,
        0,
        &computeQueue_
    );

    vkGetDeviceQueue(
        device_,
        transferFamily,
        0,
        &transferQueue_
    );

    initialized_ = true;

    return true;
}

void VulkanDevice::Shutdown() {
    if (device_ != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(device_);
        vkDestroyDevice(device_, nullptr);
    }

    device_ = VK_NULL_HANDLE;
    graphicsQueue_ = VK_NULL_HANDLE;
    computeQueue_ = VK_NULL_HANDLE;
    transferQueue_ = VK_NULL_HANDLE;

    initialized_ = false;
}

VkDevice VulkanDevice::Get() const {
    return device_;
}

VkQueue VulkanDevice::GetGraphicsQueue() const {
    return graphicsQueue_;
}

VkQueue VulkanDevice::GetComputeQueue() const {
    return computeQueue_;
}

VkQueue VulkanDevice::GetTransferQueue() const {
    return transferQueue_;
}

bool VulkanDevice::IsValid() const {
    return initialized_ &&
           device_ != VK_NULL_HANDLE;
}

}
