#include "OrbitVulkanInstance.h"

#include <vector>
#include <cstring>

namespace OrbitGPU {

VulkanInstance::VulkanInstance()
    : instance_(VK_NULL_HANDLE),
      initialized_(false) {
}

VulkanInstance::~VulkanInstance() {
    Shutdown();
}

bool VulkanInstance::Initialize(const std::string& applicationName,
                                uint32_t applicationVersion) {
    if (initialized_) {
        return true;
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName.c_str();
    appInfo.applicationVersion = applicationVersion;
    appInfo.pEngineName = "Project Orbit";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> extensions;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames =
        extensions.empty() ? nullptr : extensions.data();

    VkResult result = vkCreateInstance(
        &createInfo,
        nullptr,
        &instance_
    );

    if (result != VK_SUCCESS) {
        instance_ = VK_NULL_HANDLE;
        initialized_ = false;
        return false;
    }

    initialized_ = true;
    return true;
}

void VulkanInstance::Shutdown() {
    if (instance_ != VK_NULL_HANDLE) {
        vkDestroyInstance(instance_, nullptr);
        instance_ = VK_NULL_HANDLE;
    }

    initialized_ = false;
}

bool VulkanInstance::IsValid() const {
    return initialized_ && instance_ != VK_NULL_HANDLE;
}

VkInstance VulkanInstance::Get() const {
    return instance_;
}

}
