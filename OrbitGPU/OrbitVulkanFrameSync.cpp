#include "OrbitVulkanFrameSync.h"

OrbitVulkanFrameSync::OrbitVulkanFrameSync()
    : device_(VK_NULL_HANDLE)
{
}

OrbitVulkanFrameSync::~OrbitVulkanFrameSync()
{
    Destroy();
}

bool OrbitVulkanFrameSync::Create(
    VkDevice device,
    uint32_t frameCount)
{
    if (device == VK_NULL_HANDLE ||
        frameCount == 0)
    {
        return false;
    }

    Destroy();

    imageAvailable_.resize(frameCount);
    renderFinished_.resize(frameCount);
    fences_.resize(frameCount);

    VkSemaphoreCreateInfo semaphoreInfo{};

    semaphoreInfo.sType =
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};

    fenceInfo.sType =
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    fenceInfo.flags =
        VK_FENCE_CREATE_SIGNALED_BIT;

    for (uint32_t i = 0;
         i < frameCount;
         ++i)
    {
        if (vkCreateSemaphore(
                device,
                &semaphoreInfo,
                nullptr,
                &imageAvailable_[i]) != VK_SUCCESS)
        {
            Destroy();
            return false;
        }

        if (vkCreateSemaphore(
                device,
                &semaphoreInfo,
                nullptr,
                &renderFinished_[i]) != VK_SUCCESS)
        {
            Destroy();
            return false;
        }

        if (vkCreateFence(
                device,
                &fenceInfo,
                nullptr,
                &fences_[i]) != VK_SUCCESS)
        {
            Destroy();
            return false;
        }
    }

    device_ = device;

    return true;
}

void OrbitVulkanFrameSync::Destroy()
{
    if (device_ != VK_NULL_HANDLE)
    {
        for (VkSemaphore semaphore :
             imageAvailable_)
        {
            if (semaphore != VK_NULL_HANDLE)
            {
                vkDestroySemaphore(
                    device_,
                    semaphore,
                    nullptr
                );
            }
        }

        for (VkSemaphore semaphore :
             renderFinished_)
        {
            if (semaphore != VK_NULL_HANDLE)
            {
                vkDestroySemaphore(
                    device_,
                    semaphore,
                    nullptr
                );
            }
        }

        for (VkFence fence : fences_)
        {
            if (fence != VK_NULL_HANDLE)
            {
                vkDestroyFence(
                    device_,
                    fence,
                    nullptr
                );
            }
        }
    }

    imageAvailable_.clear();
    renderFinished_.clear();
    fences_.clear();

    device_ = VK_NULL_HANDLE;
}

VkSemaphore
OrbitVulkanFrameSync::GetImageAvailable(
    uint32_t frame) const
{
    if (frame >= imageAvailable_.size())
        return VK_NULL_HANDLE;

    return imageAvailable_[frame];
}

VkSemaphore
OrbitVulkanFrameSync::GetRenderFinished(
    uint32_t frame) const
{
    if (frame >= renderFinished_.size())
        return VK_NULL_HANDLE;

    return renderFinished_[frame];
}

VkFence OrbitVulkanFrameSync::GetFence(
    uint32_t frame) const
{
    if (frame >= fences_.size())
        return VK_NULL_HANDLE;

    return fences_[frame];
}

uint32_t OrbitVulkanFrameSync::GetFrameCount() const
{
    return static_cast<uint32_t>(
        fences_.size());
}

bool OrbitVulkanFrameSync::IsValid() const
{
    return device_ != VK_NULL_HANDLE &&
           !fences_.empty();
}
