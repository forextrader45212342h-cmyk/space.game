#include "OrbitVulkanSwapchain.h"

#include <algorithm>
#include <limits>

namespace OrbitGPU {

VulkanSwapchain::VulkanSwapchain()
    : instance_(VK_NULL_HANDLE),
      physicalDevice_(VK_NULL_HANDLE),
      device_(VK_NULL_HANDLE),
      surface_(VK_NULL_HANDLE),
      swapchain_(VK_NULL_HANDLE),
      format_(VK_FORMAT_UNDEFINED),
      colorSpace_(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR),
      extent_{},
      graphicsQueueFamily_(UINT32_MAX),
      presentQueueFamily_(UINT32_MAX),
      initialized_(false) {
}

bool VulkanSwapchain::FindQueueFamilies() {
    uint32_t count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice_,
        &count,
        nullptr
    );

    if (count == 0) {
        return false;
    }

    std::vector<VkQueueFamilyProperties> families(count);

    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice_,
        &count,
        families.data()
    );

    graphicsQueueFamily_ = UINT32_MAX;
    presentQueueFamily_ = UINT32_MAX;

    for (uint32_t i = 0; i < count; ++i) {

        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueFamily_ = i;
        }

        VkBool32 presentSupported = VK_FALSE;

        if (vkGetPhysicalDeviceSurfaceSupportKHR(
                physicalDevice_,
                i,
                surface_,
                &presentSupported) == VK_SUCCESS &&
            presentSupported) {

            presentQueueFamily_ = i;
        }
    }

    return graphicsQueueFamily_ != UINT32_MAX &&
           presentQueueFamily_ != UINT32_MAX;
}

VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& formats
) const {

    for (const auto& format : formats) {

        if (format.format ==
                VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace ==
                VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {

            return format;
        }
    }

    return formats[0];
}

VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(
    const std::vector<VkPresentModeKHR>& modes
) const {

    for (const auto mode : modes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchain::ChooseExtent(
    const VkSurfaceCapabilitiesKHR& capabilities,
    uint32_t width,
    uint32_t height
) const {

    if (capabilities.currentExtent.width !=
            std::numeric_limits<uint32_t>::max()) {

        return capabilities.currentExtent;
    }

    VkExtent2D result{width, height};

    result.width = std::clamp(
        result.width,
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width
    );

    result.height = std::clamp(
        result.height,
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height
    );

    return result;
}

bool VulkanSwapchain::Initialize(
    VkInstance instance,
    VkPhysicalDevice physicalDevice,
    VkDevice device,
    VkSurfaceKHR surface,
    uint32_t width,
    uint32_t height
) {

    if (instance == VK_NULL_HANDLE ||
        physicalDevice == VK_NULL_HANDLE ||
        device == VK_NULL_HANDLE ||
        surface == VK_NULL_HANDLE) {
        return false;
    }

    instance_ = instance;
    physicalDevice_ = physicalDevice;
    device_ = device;
    surface_ = surface;

    if (!FindQueueFamilies()) {
        return false;
    }

    VkSurfaceCapabilitiesKHR capabilities{};

    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            physicalDevice_,
            surface_,
            &capabilities) != VK_SUCCESS) {

        return false;
    }

    uint32_t formatCount = 0;

    vkGetPhysicalDeviceSurfaceFormatsKHR(
        physicalDevice_,
        surface_,
        &formatCount,
        nullptr
    );

    if (formatCount == 0) {
        return false;
    }

    std::vector<VkSurfaceFormatKHR> formats(
        formatCount
    );

    vkGetPhysicalDeviceSurfaceFormatsKHR(
        physicalDevice_,
        surface_,
        &formatCount,
        formats.data()
    );

    uint32_t presentModeCount = 0;

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        physicalDevice_,
        surface_,
        &presentModeCount,
        nullptr
    );

    if (presentModeCount == 0) {
        return false;
    }

    std::vector<VkPresentModeKHR> presentModes(
        presentModeCount
    );

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        physicalDevice_,
        surface_,
        &presentModeCount,
        presentModes.data()
    );

    const VkSurfaceFormatKHR surfaceFormat =
        ChooseSurfaceFormat(formats);

    format_ = surfaceFormat.format;
    colorSpace_ = surfaceFormat.colorSpace;

    extent_ = ChooseExtent(
        capabilities,
        width,
        height
    );

    uint32_t imageCount =
        capabilities.minImageCount + 1;

    if (capabilities.maxImageCount > 0 &&
        imageCount > capabilities.maxImageCount) {

        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR info{};
    info.sType =
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

    info.surface = surface_;

    info.minImageCount = imageCount;
    info.imageFormat = format_;
    info.imageColorSpace = colorSpace_;
    info.imageExtent = extent_;
    info.imageArrayLayers = 1;

    info.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilies[] = {
        graphicsQueueFamily_,
        presentQueueFamily_
    };

    if (graphicsQueueFamily_ != presentQueueFamily_) {

        info.imageSharingMode =
            VK_SHARING_MODE_CONCURRENT;

        info.queueFamilyIndexCount = 2;
        info.pQueueFamilyIndices = queueFamilies;

    } else {

        info.imageSharingMode =
            VK_SHARING_MODE_EXCLUSIVE;
    }

    info.preTransform =
        capabilities.currentTransform;

    info.compositeAlpha =
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    info.presentMode =
        ChoosePresentMode(presentModes);

    info.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(
            device_,
            &info,
            nullptr,
            &swapchain_) != VK_SUCCESS) {

        swapchain_ = VK_NULL_HANDLE;
        return false;
    }

    uint32_t actualImageCount = 0;

    vkGetSwapchainImagesKHR(
        device_,
        swapchain_,
        &actualImageCount,
        nullptr
    );

    images_.resize(actualImageCount);

    vkGetSwapchainImagesKHR(
        device_,
        swapchain_,
        &actualImageCount,
        images_.data()
    );

    initialized_ = true;

    return true;
}

bool VulkanSwapchain::Recreate(
    uint32_t width,
    uint32_t height
) {

    if (!initialized_) {
        return false;
    }

    vkDeviceWaitIdle(device_);

    if (swapchain_ != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(
            device_,
            swapchain_,
            nullptr
        );

        swapchain_ = VK_NULL_HANDLE;
    }

    images_.clear();

    initialized_ = false;

    return Initialize(
        instance_,
        physicalDevice_,
        device_,
        surface_,
        width,
        height
    );
}

void VulkanSwapchain::Shutdown() {

    if (device_ != VK_NULL_HANDLE &&
        swapchain_ != VK_NULL_HANDLE) {

        vkDeviceWaitIdle(device_);

        vkDestroySwapchainKHR(
            device_,
            swapchain_,
            nullptr
        );
    }

    swapchain_ = VK_NULL_HANDLE;
    images_.clear();

    instance_ = VK_NULL_HANDLE;
    physicalDevice_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
    surface_ = VK_NULL_HANDLE;

    initialized_ = false;
}

VkSwapchainKHR VulkanSwapchain::Get() const {
    return swapchain_;
}

VkFormat VulkanSwapchain::GetFormat() const {
    return format_;
}

VkExtent2D VulkanSwapchain::GetExtent() const {
    return extent_;
}

const std::vector<VkImage>&
VulkanSwapchain::GetImages() const {
    return images_;
}

bool VulkanSwapchain::IsValid() const {
    return initialized_ &&
           swapchain_ != VK_NULL_HANDLE &&
           !images_.empty();
}

}
