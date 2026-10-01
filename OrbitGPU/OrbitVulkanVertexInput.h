#pragma once

#include <vulkan/vulkan.h>
#include <vector>

struct OrbitVertex
{
    float position[3];
    float normal[3];
    float uv[2];
};

class OrbitVulkanVertexInput
{
public:
    static VkVertexInputBindingDescription Binding();

    static std::vector<VkVertexInputAttributeDescription>
    Attributes();
};
