#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace OrbitGPU {

class VulkanSwapchain {
public:
    VulkanSwapchain();

    bool Initialize(
        VkInstance instance,
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        VkSurfaceKHR surface,
        uint32_t width,
        uint32_t height
    );

    void Shutdown();

    bool Recreate(
        uint32_t width,
        uint32_t height
    );

    VkSwapchainKHR Get() const;
    VkFormat GetFormat() const;
    VkExtent2D GetExtent() const;

    const std::vector<VkImage>& GetImages() const;

    bool IsValid() const;

private:
    VkInstance instance_;
    VkPhysicalDevice physicalDevice_;
    VkDevice device_;
    VkSurfaceKHR surface_;

    VkSwapchainKHR swapchain_;

    VkFormat format_;
    VkColorSpaceKHR colorSpace_;
    VkExtent2D extent_;

    std::vector<VkImage> images_;

    uint32_t graphicsQueueFamily_;
    uint32_t presentQueueFamily_;

    bool initialized_;

    bool FindQueueFamilies();

    VkSurfaceFormatKHR ChooseSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& formats
    ) const;

    VkPresentModeKHR ChoosePresentMode(
        const std::vector<VkPresentModeKHR>& modes
    ) const;

    VkExtent2D ChooseExtent(
        const VkSurfaceCapabilitiesKHR& capabilities,
        uint32_t width,
        uint32_t height
    ) const;
};

}
