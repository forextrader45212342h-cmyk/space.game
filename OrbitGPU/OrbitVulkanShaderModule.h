#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace OrbitGPU {

class VulkanShaderModule {
public:
    VulkanShaderModule();

    bool Initialize(
        VkDevice device,
        const std::vector<uint32_t>& spirv
    );

    void Shutdown();

    VkShaderModule Get() const;

    bool IsValid() const;

private:
    VkDevice device_;
    VkShaderModule shaderModule_;
    bool initialized_;
};

}
