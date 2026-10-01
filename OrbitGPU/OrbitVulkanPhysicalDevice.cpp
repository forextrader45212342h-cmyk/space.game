#include "OrbitVulkanPhysicalDevice.h"

#include <limits>

namespace OrbitGPU {

VulkanPhysicalDevice::VulkanPhysicalDevice()
    : physicalDevice_(VK_NULL_HANDLE),
      properties_{},
      features_{},
      memoryProperties_{},
      graphicsQueueFamily_(UINT32_MAX),
      computeQueueFamily_(UINT32_MAX),
      transferQueueFamily_(UINT32_MAX),
      initialized_(false) {
}

bool VulkanPhysicalDevice::Select(VkInstance instance) {
    if (instance == VK_NULL_HANDLE) {
        return false;
    }

    uint32_t deviceCount = 0;

    if (vkEnumeratePhysicalDevices(
            instance,
            &deviceCount,
            nullptr) != VK_SUCCESS) {
        return false;
    }

    if (deviceCount == 0) {
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);

    if (vkEnumeratePhysicalDevices(
            instance,
            &deviceCount,
            devices.data()) != VK_SUCCESS) {
        return false;
    }

    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    int bestScore = -1;

    for (VkPhysicalDevice device : devices) {
        VkPhysicalDeviceProperties props{};
        VkPhysicalDeviceFeatures features{};

        vkGetPhysicalDeviceProperties(device, &props);
        vkGetPhysicalDeviceFeatures(device, &features);

        int score = 0;

        if (props.deviceType ==
            VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        if (props.deviceType ==
            VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            score += 500;
        }

        if (props.deviceType ==
            VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
            score += 250;
        }

        if (props.deviceType ==
            VK_PHYSICAL_DEVICE_TYPE_CPU) {
            score += 100;
        }

        score += static_cast<int>(props.limits.maxImageDimension2D);

        if (score > bestScore) {
            bestScore = score;
            bestDevice = device;
        }
    }

    if (bestDevice == VK_NULL_HANDLE) {
        return false;
    }

    physicalDevice_ = bestDevice;

    vkGetPhysicalDeviceProperties(
        physicalDevice_,
        &properties_
    );

    vkGetPhysicalDeviceFeatures(
        physicalDevice_,
        &features_
    );

    vkGetPhysicalDeviceMemoryProperties(
        physicalDevice_,
        &memoryProperties_
    );

    if (!FindQueueFamilies(physicalDevice_)) {
        physicalDevice_ = VK_NULL_HANDLE;
        return false;
    }

    initialized_ = true;
    return true;
}

bool VulkanPhysicalDevice::FindQueueFamilies(VkPhysicalDevice device) {
    uint32_t count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &count,
        nullptr
    );

    if (count == 0) {
        return false;
    }

    std::vector<VkQueueFamilyProperties> families(count);

    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &count,
        families.data()
    );

    graphicsQueueFamily_ = UINT32_MAX;
    computeQueueFamily_ = UINT32_MAX;
    transferQueueFamily_ = UINT32_MAX;

    for (uint32_t i = 0; i < count; ++i) {
        const auto flags = families[i].queueFlags;

        if ((flags & VK_QUEUE_GRAPHICS_BIT) &&
            graphicsQueueFamily_ == UINT32_MAX) {
            graphicsQueueFamily_ = i;
        }

        if ((flags & VK_QUEUE_COMPUTE_BIT) &&
            computeQueueFamily_ == UINT32_MAX) {
            computeQueueFamily_ = i;
        }

        if ((flags & VK_QUEUE_TRANSFER_BIT) &&
            transferQueueFamily_ == UINT32_MAX) {
            transferQueueFamily_ = i;
        }
    }

    if (graphicsQueueFamily_ == UINT32_MAX) {
        return false;
    }

    if (computeQueueFamily_ == UINT32_MAX) {
        computeQueueFamily_ = graphicsQueueFamily_;
    }

    if (transferQueueFamily_ == UINT32_MAX) {
        transferQueueFamily_ = graphicsQueueFamily_;
    }

    return true;
}

VkPhysicalDevice VulkanPhysicalDevice::Get() const {
    return physicalDevice_;
}

const VkPhysicalDeviceProperties&
VulkanPhysicalDevice::GetProperties() const {
    return properties_;
}

const VkPhysicalDeviceFeatures&
VulkanPhysicalDevice::GetFeatures() const {
    return features_;
}

const VkPhysicalDeviceMemoryProperties&
VulkanPhysicalDevice::GetMemoryProperties() const {
    return memoryProperties_;
}

uint32_t VulkanPhysicalDevice::GetGraphicsQueueFamily() const {
    return graphicsQueueFamily_;
}

uint32_t VulkanPhysicalDevice::GetComputeQueueFamily() const {
    return computeQueueFamily_;
}

uint32_t VulkanPhysicalDevice::GetTransferQueueFamily() const {
    return transferQueueFamily_;
}

bool VulkanPhysicalDevice::IsValid() const {
    return initialized_ &&
           physicalDevice_ != VK_NULL_HANDLE;
}

std::string VulkanPhysicalDevice::GetDeviceName() const {
    if (!IsValid()) {
        return "Unknown";
    }

    return properties_.deviceName;
}

}
