#include "OrbitVulkanFrameRenderer.h"

VkResult OrbitVulkanFrameRenderer::Acquire(
    VkDevice device,
    VkSwapchainKHR swapchain,
    VkSemaphore imageAvailable,
    uint32_t& imageIndex)
{
    if (device == VK_NULL_HANDLE ||
        swapchain == VK_NULL_HANDLE ||
        imageAvailable == VK_NULL_HANDLE)
    {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    return vkAcquireNextImageKHR(
        device,
        swapchain,
        UINT64_MAX,
        imageAvailable,
        VK_NULL_HANDLE,
        &imageIndex
    );
}

bool OrbitVulkanFrameRenderer::DrawTriangle(
    VkCommandBuffer commandBuffer,
    VkPipeline pipeline,
    VkPipelineLayout layout,
    VkExtent2D extent)
{
    if (commandBuffer == VK_NULL_HANDLE ||
        pipeline == VK_NULL_HANDLE)
    {
        return false;
    }

    vkCmdBindPipeline(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline
    );

    VkViewport viewport{};

    viewport.x = 0.0f;
    viewport.y = 0.0f;

    viewport.width =
        static_cast<float>(extent.width);

    viewport.height =
        static_cast<float>(extent.height);

    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetViewport(
        commandBuffer,
        0,
        1,
        &viewport
    );

    VkRect2D scissor{};

    scissor.offset = {0, 0};
    scissor.extent = extent;

    vkCmdSetScissor(
        commandBuffer,
        0,
        1,
        &scissor
    );

    vkCmdDraw(
        commandBuffer,
        3,
        1,
        0,
        0
    );

    return true;
}
