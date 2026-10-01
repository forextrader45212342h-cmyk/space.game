#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class OrbitVulkanSwapchainImageViews
{
public:
    OrbitVulkanSwapchainImageViews();
    ~OrbitVulkanSwapchainImageViews();

    bool Create(
        VkDevice device,
        const std::vector<VkImage>& images,
        VkFormat format
    );

    void Destroy();

    const std::vector<VkImageView>& GetViews() const;

    bool IsValid() const;

private:
    VkDevice device_;
    std::vector<VkImageView> views_;
};
