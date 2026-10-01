#include "OrbitVulkanPipelineLayout.h"

OrbitVulkanPipelineLayout::
OrbitVulkanPipelineLayout()
    : device_(VK_NULL_HANDLE),
      pipelineLayout_(VK_NULL_HANDLE)
{
}

OrbitVulkanPipelineLayout::
~OrbitVulkanPipelineLayout()
{
    Destroy();
}

bool OrbitVulkanPipelineLayout::Create(
    VkDevice device,
    const std::vector<VkDescriptorSetLayout>& layouts)
{
    if (device == VK_NULL_HANDLE)
        return false;

    Destroy();

    VkPipelineLayoutCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    info.setLayoutCount =
        static_cast<uint32_t>(layouts.size());

    info.pSetLayouts =
        layouts.empty() ? nullptr : layouts.data();

    if (vkCreatePipelineLayout(
            device,
            &info,
            nullptr,
            &pipelineLayout_) != VK_SUCCESS)
    {
        pipelineLayout_ = VK_NULL_HANDLE;
        return false;
    }

    device_ = device;
    return true;
}

void OrbitVulkanPipelineLayout::Destroy()
{
    if (device_ != VK_NULL_HANDLE &&
        pipelineLayout_ != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(
            device_,
            pipelineLayout_,
            nullptr
        );
    }

    pipelineLayout_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkPipelineLayout
OrbitVulkanPipelineLayout::Get() const
{
    return pipelineLayout_;
}

bool OrbitVulkanPipelineLayout::IsValid() const
{
    return pipelineLayout_ != VK_NULL_HANDLE;
}
