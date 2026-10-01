#include "OrbitVulkanCommandRecorder.h"

bool OrbitVulkanCommandRecorder::Begin(
    VkCommandBuffer commandBuffer)
{
    if (commandBuffer == VK_NULL_HANDLE)
        return false;

    VkCommandBufferBeginInfo info{};

    info.sType =
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    info.flags =
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    return vkBeginCommandBuffer(
        commandBuffer,
        &info
    ) == VK_SUCCESS;
}

bool OrbitVulkanCommandRecorder::BeginRenderPass(
    VkCommandBuffer commandBuffer,
    VkRenderPass renderPass,
    VkFramebuffer framebuffer,
    VkExtent2D extent)
{
    if (commandBuffer == VK_NULL_HANDLE ||
        renderPass == VK_NULL_HANDLE ||
        framebuffer == VK_NULL_HANDLE)
    {
        return false;
    }

    VkClearValue clearValues[2]{};

    clearValues[0].color =
    {
        {0.005f, 0.008f, 0.02f, 1.0f}
    };

    clearValues[1].depthStencil =
    {
        1.0f,
        0
    };

    VkRenderPassBeginInfo info{};

    info.sType =
        VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

    info.renderPass = renderPass;
    info.framebuffer = framebuffer;

    info.renderArea.offset = {0, 0};
    info.renderArea.extent = extent;

    info.clearValueCount = 2;
    info.pClearValues = clearValues;

    vkCmdBeginRenderPass(
        commandBuffer,
        &info,
        VK_SUBPASS_CONTENTS_INLINE
    );

    return true;
}

bool OrbitVulkanCommandRecorder::EndRenderPass(
    VkCommandBuffer commandBuffer)
{
    if (commandBuffer == VK_NULL_HANDLE)
        return false;

    vkCmdEndRenderPass(commandBuffer);

    return true;
}

bool OrbitVulkanCommandRecorder::End(
    VkCommandBuffer commandBuffer)
{
    if (commandBuffer == VK_NULL_HANDLE)
        return false;

    return vkEndCommandBuffer(
        commandBuffer
    ) == VK_SUCCESS;
}
