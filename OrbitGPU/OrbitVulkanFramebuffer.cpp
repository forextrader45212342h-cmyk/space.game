#include "OrbitVulkanFramebuffer.h"

OrbitVulkanFramebuffer::OrbitVulkanFramebuffer()
    : device_(VK_NULL_HANDLE)
{
}

OrbitVulkanFramebuffer::~OrbitVulkanFramebuffer()
{
    Destroy();
}

bool OrbitVulkanFramebuffer::Create(
    VkDevice device,
    VkRenderPass renderPass,
    const std::vector<VkImageView>& colorViews,
    VkImageView depthView,
    VkExtent2D extent)
{
    if (device == VK_NULL_HANDLE ||
        renderPass == VK_NULL_HANDLE ||
        colorViews.empty() ||
        depthView == VK_NULL_HANDLE)
    {
        return false;
    }

    Destroy();

    framebuffers_.resize(colorViews.size());

    for (size_t i = 0; i < colorViews.size(); ++i)
    {
        VkImageView attachments[2] =
        {
            colorViews[i],
            depthView
        };

        VkFramebufferCreateInfo info{};

        info.sType =
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

        info.renderPass = renderPass;

        info.attachmentCount = 2;
        info.pAttachments = attachments;

        info.width = extent.width;
        info.height = extent.height;

        info.layers = 1;

        if (vkCreateFramebuffer(
                device,
                &info,
                nullptr,
                &framebuffers_[i]) != VK_SUCCESS)
        {
            Destroy();
            return false;
        }
    }

    device_ = device;

    return true;
}

void OrbitVulkanFramebuffer::Destroy()
{
    if (device_ != VK_NULL_HANDLE)
    {
        for (VkFramebuffer framebuffer :
             framebuffers_)
        {
            if (framebuffer != VK_NULL_HANDLE)
            {
                vkDestroyFramebuffer(
                    device_,
                    framebuffer,
                    nullptr
                );
            }
        }
    }

    framebuffers_.clear();
    device_ = VK_NULL_HANDLE;
}

const std::vector<VkFramebuffer>&
OrbitVulkanFramebuffer::Get() const
{
    return framebuffers_;
}

bool OrbitVulkanFramebuffer::IsValid() const
{
    return !framebuffers_.empty();
}
