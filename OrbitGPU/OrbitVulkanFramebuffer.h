#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class OrbitVulkanFramebuffer
{
public:
    OrbitVulkanFramebuffer();
    ~OrbitVulkanFramebuffer();

    bool Create(
        VkDevice device,
        VkRenderPass renderPass,
        const std::vector<VkImageView>& colorViews,
        VkImageView depthView,
        VkExtent2D extent
    );

    void Destroy();

    const std::vector<VkFramebuffer>& Get() const;

    bool IsValid() const;

private:
    VkDevice device_;
    std::vector<VkFramebuffer> framebuffers_;
};
