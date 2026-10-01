#include "OrbitVulkanDescriptorPool.h"

OrbitVulkanDescriptorPool::
OrbitVulkanDescriptorPool()
    : device_(VK_NULL_HANDLE),
      pool_(VK_NULL_HANDLE)
{
}

OrbitVulkanDescriptorPool::
~OrbitVulkanDescriptorPool()
{
    Destroy();
}

bool OrbitVulkanDescriptorPool::Create(
    VkDevice device,
    const std::vector<VkDescriptorPoolSize>& sizes,
    uint32_t maxSets)
{
    if (device == VK_NULL_HANDLE ||
        maxSets == 0)
    {
        return false;
    }

    Destroy();

    VkDescriptorPoolCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

    info.maxSets = maxSets;

    info.poolSizeCount =
        static_cast<uint32_t>(sizes.size());

    info.pPoolSizes =
        sizes.empty() ? nullptr : sizes.data();

    if (vkCreateDescriptorPool(
            device,
            &info,
            nullptr,
            &pool_) != VK_SUCCESS)
    {
        pool_ = VK_NULL_HANDLE;
        return false;
    }

    device_ = device;
    return true;
}

void OrbitVulkanDescriptorPool::Destroy()
{
    if (device_ != VK_NULL_HANDLE &&
        pool_ != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(
            device_,
            pool_,
            nullptr
        );
    }

    pool_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkDescriptorPool OrbitVulkanDescriptorPool::Get() const
{
    return pool_;
}

bool OrbitVulkanDescriptorPool::IsValid() const
{
    return pool_ != VK_NULL_HANDLE;
}
