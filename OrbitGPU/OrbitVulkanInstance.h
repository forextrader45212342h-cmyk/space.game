#pragma once

#include <vulkan/vulkan.h>
#include <string>

namespace OrbitGPU {

class VulkanInstance {
public:
    VulkanInstance();
    ~VulkanInstance();

    bool Initialize(const std::string& applicationName,
                    uint32_t applicationVersion = VK_MAKE_VERSION(1, 0, 0));

    void Shutdown();

    bool IsValid() const;
    VkInstance Get() const;

private:
    VkInstance instance_;
    bool initialized_;
};

}
