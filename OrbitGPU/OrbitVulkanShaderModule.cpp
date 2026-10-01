#include "OrbitVulkanShaderModule.h"

namespace OrbitGPU {

VulkanShaderModule::VulkanShaderModule()
    : device_(VK_NULL_HANDLE),
      shaderModule_(VK_NULL_HANDLE),
      initialized_(false) {
}

bool VulkanShaderModule::Initialize(
    VkDevice device,
    const std::vector<uint32_t>& spirv) {

    if (device == VK_NULL_HANDLE ||
        spirv.empty()) {
        return false;
    }

    device_ = device;

    VkShaderModuleCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    info.codeSize =
        spirv.size() * sizeof(uint32_t);

    info.pCode = spirv.data();

    VkResult result = vkCreateShaderModule(
        device_,
        &info,
        nullptr,
        &shaderModule_
    );

    if (result != VK_SUCCESS) {
        shaderModule_ = VK_NULL_HANDLE;
        return false;
    }

    initialized_ = true;

    return true;
}

void VulkanShaderModule::Shutdown() {
    if (shaderModule_ != VK_NULL_HANDLE &&
        device_ != VK_NULL_HANDLE) {

        vkDestroyShaderModule(
            device_,
            shaderModule_,
            nullptr
        );
    }

    shaderModule_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
    initialized_ = false;
}

VkShaderModule VulkanShaderModule::Get() const {
    return shaderModule_;
}

bool VulkanShaderModule::IsValid() const {
    return initialized_ &&
           shaderModule_ != VK_NULL_HANDLE;
}

}
