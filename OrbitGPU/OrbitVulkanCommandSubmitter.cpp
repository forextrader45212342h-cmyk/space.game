#include "OrbitVulkanCommandSubmitter.h"

bool OrbitVulkanCommandSubmitter::Submit(
    VkQueue queue,
    VkCommandBuffer commandBuffer,
    VkSemaphore waitSemaphore,
    VkSemaphore signalSemaphore,
    VkFence fence)
{
    if (queue == VK_NULL_HANDLE ||
        commandBuffer == VK_NULL_HANDLE)
    {
        return false;
    }

    VkPipelineStageFlags waitStage =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit{};

    submit.sType =
        VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submit.waitSemaphoreCount =
        waitSemaphore != VK_NULL_HANDLE ? 1 : 0;

    submit.pWaitSemaphores =
        waitSemaphore != VK_NULL_HANDLE
            ? &waitSemaphore
            : nullptr;

    submit.pWaitDstStageMask =
        waitSemaphore != VK_NULL_HANDLE
            ? &waitStage
            : nullptr;

    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &commandBuffer;

    submit.signalSemaphoreCount =
        signalSemaphore != VK_NULL_HANDLE ? 1 : 0;

    submit.pSignalSemaphores =
        signalSemaphore != VK_NULL_HANDLE
            ? &signalSemaphore
            : nullptr;

    return vkQueueSubmit(
        queue,
        1,
        &submit,
        fence
    ) == VK_SUCCESS;
}

bool OrbitVulkanCommandSubmitter::Present(
    VkQueue queue,
    VkSwapchainKHR swapchain,
    uint32_t imageIndex,
    VkSemaphore waitSemaphore)
{
    if (queue == VK_NULL_HANDLE ||
        swapchain == VK_NULL_HANDLE)
    {
        return false;
    }

    VkPresentInfoKHR present{};

    present.sType =
        VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    present.waitSemaphoreCount =
        waitSemaphore != VK_NULL_HANDLE ? 1 : 0;

    present.pWaitSemaphores =
        waitSemaphore != VK_NULL_HANDLE
            ? &waitSemaphore
            : nullptr;

    present.swapchainCount = 1;
    present.pSwapchains = &swapchain;
    present.pImageIndices = &imageIndex;

    VkResult result =
        vkQueuePresentKHR(
            queue,
            &present
        );

    return result == VK_SUCCESS ||
           result == VK_SUBOPTIMAL_KHR;
}
