#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class OrbitVulkanFrameSync
{
public:
    OrbitVulkanFrameSync();
    ~OrbitVulkanFrameSync();

    bool Create(
        VkDevice device,
        uint32_t frameCount
    );

    void Destroy();

    VkSemaphore GetImageAvailable(
        uint32_t frame
    ) const;

    VkSemaphore GetRenderFinished(
        uint32_t frame
    ) const;

    VkFence GetFence(
        uint32_t frame
    ) const;

    uint32_t GetFrameCount() const;

    bool IsValid() const;

private:
    VkDevice device_;

    std::vector<VkSemaphore>
        imageAvailable_;

    std::vector<VkSemaphore>
        renderFinished_;

    std::vector<VkFence>
        fences_;
};
