#include "OrbitVulkanDescriptorSet.h"

OrbitVulkanDescriptorSet::
OrbitVulkanDescriptorSet()
    : descriptorSet_(VK_NULL_HANDLE)
{
}

bool OrbitVulkanDescriptorSet::Allocate(
    VkDevice device,
    VkDescriptorPool pool,
    VkDescriptorSetLayout layout)
{
    if (device == VK_NULL_HANDLE ||
        pool == VK_NULL_HANDLE ||
        layout == VK_NULL_HANDLE)
    {
        return false;
    }

    Reset();

    VkDescriptorSetAllocateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

    info.descriptorPool = pool;
    info.descriptorSetCount = 1;
    info.pSetLayouts = &layout;

    if (vkAllocateDescriptorSets(
            device,
            &info,
            &descriptorSet_) != VK_SUCCESS)
    {
        descriptorSet_ = VK_NULL_HANDLE;
        return false;
    }

    return true;
}

void OrbitVulkanDescriptorSet::Reset()
{
    descriptorSet_ = VK_NULL_HANDLE;
}

VkDescriptorSet OrbitVulkanDescriptorSet::Get() const
{
    return descriptorSet_;
}

bool OrbitVulkanDescriptorSet::IsValid() const
{
    return descriptorSet_ != VK_NULL_HANDLE;
}
