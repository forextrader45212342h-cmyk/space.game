#include "OrbitVulkanRenderPass.h"

OrbitVulkanRenderPass::OrbitVulkanRenderPass()
    : device_(VK_NULL_HANDLE),
      renderPass_(VK_NULL_HANDLE)
{
}

OrbitVulkanRenderPass::~OrbitVulkanRenderPass()
{
    Destroy();
}

bool OrbitVulkanRenderPass::Create(
    VkDevice device,
    VkFormat colorFormat,
    VkFormat depthFormat)
{
    if (device == VK_NULL_HANDLE)
        return false;

    Destroy();

    VkAttachmentDescription color{};
    color.format = colorFormat;
    color.samples = VK_SAMPLE_COUNT_1_BIT;

    color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    color.stencilLoadOp =
        VK_ATTACHMENT_LOAD_OP_DONT_CARE;

    color.stencilStoreOp =
        VK_ATTACHMENT_STORE_OP_DONT_CARE;

    color.initialLayout =
        VK_IMAGE_LAYOUT_UNDEFINED;

    color.finalLayout =
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depth{};
    depth.format = depthFormat;
    depth.samples = VK_SAMPLE_COUNT_1_BIT;

    depth.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    depth.stencilLoadOp =
        VK_ATTACHMENT_LOAD_OP_DONT_CARE;

    depth.stencilStoreOp =
        VK_ATTACHMENT_STORE_OP_DONT_CARE;

    depth.initialLayout =
        VK_IMAGE_LAYOUT_UNDEFINED;

    depth.finalLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorRef{};
    colorRef.attachment = 0;
    colorRef.layout =
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthRef{};
    depthRef.attachment = 1;
    depthRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint =
        VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorRef;

    subpass.pDepthStencilAttachment =
        &depthRef;

    VkSubpassDependency dependency{};

    dependency.srcSubpass =
        VK_SUBPASS_EXTERNAL;

    dependency.dstSubpass = 0;

    dependency.srcStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

    dependency.dstStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

    dependency.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkAttachmentDescription attachments[2] =
    {
        color,
        depth
    };

    VkRenderPassCreateInfo info{};

    info.sType =
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    info.attachmentCount = 2;
    info.pAttachments = attachments;

    info.subpassCount = 1;
    info.pSubpasses = &subpass;

    info.dependencyCount = 1;
    info.pDependencies = &dependency;

    if (vkCreateRenderPass(
            device,
            &info,
            nullptr,
            &renderPass_) != VK_SUCCESS)
    {
        renderPass_ = VK_NULL_HANDLE;
        return false;
    }

    device_ = device;

    return true;
}

void OrbitVulkanRenderPass::Destroy()
{
    if (device_ != VK_NULL_HANDLE &&
        renderPass_ != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(
            device_,
            renderPass_,
            nullptr
        );
    }

    renderPass_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkRenderPass OrbitVulkanRenderPass::Get() const
{
    return renderPass_;
}

bool OrbitVulkanRenderPass::IsValid() const
{
    return renderPass_ != VK_NULL_HANDLE;
}
