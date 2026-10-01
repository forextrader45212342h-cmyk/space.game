#include "OrbitVulkanVertexInput.h"

VkVertexInputBindingDescription
OrbitVulkanVertexInput::Binding()
{
    VkVertexInputBindingDescription binding{};

    binding.binding = 0;
    binding.stride = sizeof(OrbitVertex);
    binding.inputRate =
        VK_VERTEX_INPUT_RATE_VERTEX;

    return binding;
}

std::vector<VkVertexInputAttributeDescription>
OrbitVulkanVertexInput::Attributes()
{
    std::vector<VkVertexInputAttributeDescription> attributes;

    VkVertexInputAttributeDescription position{};
    position.location = 0;
    position.binding = 0;
    position.format = VK_FORMAT_R32G32B32_SFLOAT;
    position.offset =
        static_cast<uint32_t>(
            offsetof(OrbitVertex, position));

    VkVertexInputAttributeDescription normal{};
    normal.location = 1;
    normal.binding = 0;
    normal.format = VK_FORMAT_R32G32B32_SFLOAT;
    normal.offset =
        static_cast<uint32_t>(
            offsetof(OrbitVertex, normal));

    VkVertexInputAttributeDescription uv{};
    uv.location = 2;
    uv.binding = 0;
    uv.format = VK_FORMAT_R32G32_SFLOAT;
    uv.offset =
        static_cast<uint32_t>(
            offsetof(OrbitVertex, uv));

    attributes.push_back(position);
    attributes.push_back(normal);
    attributes.push_back(uv);

    return attributes;
}
