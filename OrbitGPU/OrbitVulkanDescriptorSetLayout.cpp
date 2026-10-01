#include "OrbitVulkanDescriptorSetLayout.h"

OrbitVulkanDescriptorSetLayout::
OrbitVulkanDescriptorSetLayout()
    : device_(VK_NULL_HANDLE),
      layout_(VK_NULL_HANDLE)
{
}

OrbitVulkanDescriptorSetLayout::
~OrbitVulkanDescriptorSetLayout()
{
    Destroy();
}

bool OrbitVulkanDescriptorSetLayout::Create(
    VkDevice device,
    const std::vector<VkDescriptorSetLayoutBinding>& bindings)
{
    if (device == VK_NULL_HANDLE)
        return false;

    Destroy();

    VkDescriptorSetLayoutCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

    info.bindingCount =
        static_cast<uint32_t>(bindings.size());

    info.pBindings =
        bindings.empty() ? nullptr : bindings.data();

    if (vkCreateDescriptorSetLayout(
            device,
            &info,
            nullptr,
            &layout_) != VK_SUCCESS)
    {
        layout_ = VK_NULL_HANDLE;
        return false;
    }

    device_ = device;
    return true;
}

void OrbitVulkanDescriptorSetLayout::Destroy()
{
    if (device_ != VK_NULL_HANDLE &&
        layout_ != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(
            device_,
            layout_,
            nullptr
        );
    }

    layout_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkDescriptorSetLayout
OrbitVulkanDescriptorSetLayout::Get() const
{
    return layout_;
}

bool OrbitVulkanDescriptorSetLayout::IsValid() const
{
    return layout_ != VK_NULL_HANDLE;
}
