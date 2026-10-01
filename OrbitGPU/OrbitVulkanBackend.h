#pragma once

#include <cstdint>
#include <string>

namespace OrbitVulkan
{
    class Backend
    {
    private:
        bool initialized = false;
        bool validation = false;

        uint32_t width = 0;
        uint32_t height = 0;

        std::string error;

#if __has_include(<vulkan/vulkan.h>)
#define ORBIT_HAS_VULKAN 1
#include <vulkan/vulkan.h>

        VkInstance instance = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;

        uint32_t graphicsQueueFamily = UINT32_MAX;
        uint32_t presentQueueFamily = UINT32_MAX;
#endif

    public:
        bool Initialize(
            uint32_t requestedWidth,
            uint32_t requestedHeight,
            bool enableValidation);

        void Shutdown();

        bool IsInitialized() const;

        const char* GetError() const;

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

#if defined(ORBIT_HAS_VULKAN)
        VkInstance GetInstance() const;
        VkPhysicalDevice GetPhysicalDevice() const;
        VkDevice GetDevice() const;
        VkQueue GetGraphicsQueue() const;
#endif
    };
}
