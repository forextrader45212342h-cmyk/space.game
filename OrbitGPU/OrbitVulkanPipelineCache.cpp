#include "OrbitVulkanPipelineCache.h"

OrbitVulkanPipelineCache::OrbitVulkanPipelineCache()
    : device_(VK_NULL_HANDLE),
      cache_(VK_NULL_HANDLE)
{
}

OrbitVulkanPipelineCache::~OrbitVulkanPipelineCache()
{
    Destroy();
}

bool OrbitVulkanPipelineCache::Create(VkDevice device)
{
    if (device == VK_NULL_HANDLE)
        return false;

    Destroy();

    VkPipelineCacheCreateInfo info{};

    info.sType =
        VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    if (vkCreatePipelineCache(
            device,
            &info,
            nullptr,
            &cache_) != VK_SUCCESS)
    {
        cache_ = VK_NULL_HANDLE;
        return false;
    }

    device_ = device;

    return true;
}

void OrbitVulkanPipelineCache::Destroy()
{
    if (device_ != VK_NULL_HANDLE &&
        cache_ != VK_NULL_HANDLE)
    {
        vkDestroyPipelineCache(
            device_,
            cache_,
            nullptr
        );
    }

    cache_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkPipelineCache OrbitVulkanPipelineCache::Get() const
{
    return cache_;
}

bool OrbitVulkanPipelineCache::IsValid() const
{
    return cache_ != VK_NULL_HANDLE;
}
