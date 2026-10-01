#include "OrbitVulkanUniformBuffer.h"

OrbitVulkanUniformBuffer::
OrbitVulkanUniformBuffer()
    : device_(VK_NULL_HANDLE),
      buffer_(VK_NULL_HANDLE),
      memory_(VK_NULL_HANDLE),
      size_(0)
{
}

OrbitVulkanUniformBuffer::
~OrbitVulkanUniformBuffer()
{
    Destroy();
}

bool OrbitVulkanUniformBuffer::FindMemoryType(
    VkPhysicalDevice physicalDevice,
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties,
    uint32_t& typeIndex)
{
    VkPhysicalDeviceMemoryProperties props{};

    vkGetPhysicalDeviceMemoryProperties(
        physicalDevice,
        &props
    );

    for (uint32_t i = 0;
         i < props.memoryTypeCount;
         ++i)
    {
        if ((typeFilter & (1u << i)) &&
            (props.memoryTypes[i].propertyFlags &
             properties) == properties)
        {
            typeIndex = i;
            return true;
        }
    }

    return false;
}

bool OrbitVulkanUniformBuffer::Create(
    VkPhysicalDevice physicalDevice,
    VkDevice device,
    VkDeviceSize size)
{
    if (physicalDevice == VK_NULL_HANDLE ||
        device == VK_NULL_HANDLE ||
        size == 0)
    {
        return false;
    }

    Destroy();

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType =
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    bufferInfo.size = size;

    bufferInfo.usage =
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    bufferInfo.sharingMode =
        VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(
            device,
            &bufferInfo,
            nullptr,
            &buffer_) != VK_SUCCESS)
    {
        buffer_ = VK_NULL_HANDLE;
        return false;
    }

    VkMemoryRequirements requirements{};

    vkGetBufferMemoryRequirements(
        device,
        buffer_,
        &requirements
    );

    uint32_t memoryType = 0;

    if (!FindMemoryType(
            physicalDevice,
            requirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            memoryType))
    {
        Destroy();
        return false;
    }

    VkMemoryAllocateInfo allocation{};
    allocation.sType =
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    allocation.allocationSize =
        requirements.size;

    allocation.memoryTypeIndex =
        memoryType;

    if (vkAllocateMemory(
            device,
            &allocation,
            nullptr,
            &memory_) != VK_SUCCESS)
    {
        Destroy();
        return false;
    }

    if (vkBindBufferMemory(
            device,
            buffer_,
            memory_,
            0) != VK_SUCCESS)
    {
        Destroy();
        return false;
    }

    device_ = device;
    size_ = size;

    return true;
}

bool OrbitVulkanUniformBuffer::Upload(
    const void* data,
    VkDeviceSize size,
    VkDeviceSize offset)
{
    if (device_ == VK_NULL_HANDLE ||
        memory_ == VK_NULL_HANDLE ||
        data == nullptr)
    {
        return false;
    }

    if (offset + size > size_)
        return false;

    void* mapped = nullptr;

    if (vkMapMemory(
            device_,
            memory_,
            offset,
            size,
            0,
            &mapped) != VK_SUCCESS)
    {
        return false;
    }

    const unsigned char* source =
        static_cast<const unsigned char*>(data);

    unsigned char* destination =
        static_cast<unsigned char*>(mapped);

    for (VkDeviceSize i = 0; i < size; ++i)
        destination[i] = source[i];

    vkUnmapMemory(device_, memory_);

    return true;
}

void OrbitVulkanUniformBuffer::Destroy()
{
    if (device_ != VK_NULL_HANDLE)
    {
        if (buffer_ != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(
                device_,
                buffer_,
                nullptr
            );
        }

        if (memory_ != VK_NULL_HANDLE)
        {
            vkFreeMemory(
                device_,
                memory_,
                nullptr
            );
        }
    }

    device_ = VK_NULL_HANDLE;
    buffer_ = VK_NULL_HANDLE;
    memory_ = VK_NULL_HANDLE;
    size_ = 0;
}

VkBuffer OrbitVulkanUniformBuffer::GetBuffer() const
{
    return buffer_;
}

VkDeviceMemory OrbitVulkanUniformBuffer::GetMemory() const
{
    return memory_;
}

VkDeviceSize OrbitVulkanUniformBuffer::GetSize() const
{
    return size_;
}

bool OrbitVulkanUniformBuffer::IsValid() const
{
    return buffer_ != VK_NULL_HANDLE &&
           memory_ != VK_NULL_HANDLE;
}
