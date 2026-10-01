#include "OrbitVulkanBackend.h"

#include <vector>
#include <cstring>

#if __has_include(<vulkan/vulkan.h>)
#include <vulkan/vulkan.h>
#define ORBIT_VULKAN_AVAILABLE 1
#endif

namespace OrbitVulkan
{
#if defined(ORBIT_VULKAN_AVAILABLE)

    static bool CheckLayer(
        const char* requested)
    {
        uint32_t count = 0;

        if (vkEnumerateInstanceLayerProperties(
                &count,
                nullptr) != VK_SUCCESS)
            return false;

        std::vector<VkLayerProperties> layers(count);

        vkEnumerateInstanceLayerProperties(
            &count,
            layers.data());

        for (const auto& layer : layers)
        {
            if (std::strcmp(
                    layer.layerName,
                    requested) == 0)
                return true;
        }

        return false;
    }

    static bool CheckExtension(
        const char* requested)
    {
        uint32_t count = 0;

        if (vkEnumerateInstanceExtensionProperties(
                nullptr,
                &count,
                nullptr) != VK_SUCCESS)
            return false;

        std::vector<VkExtensionProperties> extensions(count);

        vkEnumerateInstanceExtensionProperties(
            nullptr,
            &count,
            extensions.data());

        for (const auto& extension : extensions)
        {
            if (std::strcmp(
                    extension.extensionName,
                    requested) == 0)
                return true;
        }

        return false;
    }

#endif

    bool Backend::Initialize(
        uint32_t requestedWidth,
        uint32_t requestedHeight,
        bool enableValidation)
    {
        width = requestedWidth;
        height = requestedHeight;
        validation = enableValidation;

        if (width == 0 || height == 0)
        {
            error = "Invalid Vulkan render size";
            return false;
        }

#if !defined(ORBIT_VULKAN_AVAILABLE)

        error =
            "Vulkan headers are not installed on this build environment.";

        return false;

#else

        std::vector<const char*> layers;

        if (validation &&
            CheckLayer("VK_LAYER_KHRONOS_validation"))
        {
            layers.push_back(
                "VK_LAYER_KHRONOS_validation");
        }

        std::vector<const char*> extensions;

        if (CheckExtension(
                VK_KHR_SURFACE_EXTENSION_NAME))
        {
            extensions.push_back(
                VK_KHR_SURFACE_EXTENSION_NAME);
        }

#if defined(__ANDROID__)
        if (CheckExtension(
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME))
        {
            extensions.push_back(
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
        }
#endif

        VkApplicationInfo appInfo{};
        appInfo.sType =
            VK_STRUCTURE_TYPE_APPLICATION_INFO;

        appInfo.pApplicationName =
            "Project Orbit";

        appInfo.applicationVersion =
            VK_MAKE_VERSION(1, 0, 0);

        appInfo.pEngineName =
            "Orbit Engine";

        appInfo.engineVersion =
            VK_MAKE_VERSION(1, 0, 0);

        appInfo.apiVersion =
            VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};

        createInfo.sType =
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        createInfo.pApplicationInfo =
            &appInfo;

        createInfo.enabledLayerCount =
            static_cast<uint32_t>(layers.size());

        createInfo.ppEnabledLayerNames =
            layers.empty() ? nullptr : layers.data();

        createInfo.enabledExtensionCount =
            static_cast<uint32_t>(extensions.size());

        createInfo.ppEnabledExtensionNames =
            extensions.empty()
                ? nullptr
                : extensions.data();

        VkResult result =
            vkCreateInstance(
                &createInfo,
                nullptr,
                &instance);

        if (result != VK_SUCCESS)
        {
            error = "vkCreateInstance failed";
            return false;
        }

        uint32_t deviceCount = 0;

        result =
            vkEnumeratePhysicalDevices(
                instance,
                &deviceCount,
                nullptr);

        if (result != VK_SUCCESS ||
            deviceCount == 0)
        {
            error = "No Vulkan physical device found";
            Shutdown();
            return false;
        }

        std::vector<VkPhysicalDevice> devices(
            deviceCount);

        vkEnumeratePhysicalDevices(
            instance,
            &deviceCount,
            devices.data());

        for (VkPhysicalDevice candidate : devices)
        {
            uint32_t queueCount = 0;

            vkGetPhysicalDeviceQueueFamilyProperties(
                candidate,
                &queueCount,
                nullptr);

            std::vector<VkQueueFamilyProperties>
                families(queueCount);

            vkGetPhysicalDeviceQueueFamilyProperties(
                candidate,
                &queueCount,
                families.data());

            uint32_t graphics = UINT32_MAX;

            for (uint32_t i = 0; i < queueCount; ++i)
            {
                if (families[i].queueCount > 0 &&
                    (families[i].queueFlags &
                     VK_QUEUE_GRAPHICS_BIT))
                {
                    graphics = i;
                    break;
                }
            }

            if (graphics != UINT32_MAX)
            {
                physicalDevice = candidate;
                graphicsQueueFamily = graphics;
                presentQueueFamily = graphics;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            error = "No graphics-capable Vulkan queue found";
            Shutdown();
            return false;
        }

        float priority = 1.0f;

        VkDeviceQueueCreateInfo queueInfo{};

        queueInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

        queueInfo.queueFamilyIndex =
            graphicsQueueFamily;

        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &priority;

        VkPhysicalDeviceFeatures features{};

        VkDeviceCreateInfo deviceInfo{};

        deviceInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        deviceInfo.pQueueCreateInfos =
            &queueInfo;

        deviceInfo.queueCreateInfoCount = 1;

        deviceInfo.pEnabledFeatures =
            &features;

        result =
            vkCreateDevice(
                physicalDevice,
                &deviceInfo,
                nullptr,
                &device);

        if (result != VK_SUCCESS)
        {
            error = "vkCreateDevice failed";
            Shutdown();
            return false;
        }

        vkGetDeviceQueue(
            device,
            graphicsQueueFamily,
            0,
            &graphicsQueue);

        presentQueue = graphicsQueue;

        initialized = true;
        error.clear();

        return true;

#endif
    }

    void Backend::Shutdown()
    {
#if defined(ORBIT_VULKAN_AVAILABLE)

        if (device != VK_NULL_HANDLE)
        {
            vkDeviceWaitIdle(device);

            vkDestroyDevice(
                device,
                nullptr);

            device = VK_NULL_HANDLE;
        }

        physicalDevice =
            VK_NULL_HANDLE;

        graphicsQueue =
            VK_NULL_HANDLE;

        presentQueue =
            VK_NULL_HANDLE;

        if (instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(
                instance,
                nullptr);

            instance = VK_NULL_HANDLE;
        }

#endif

        initialized = false;
    }

    bool Backend::IsInitialized() const
    {
        return initialized;
    }

    const char* Backend::GetError() const
    {
        return error.c_str();
    }

    uint32_t Backend::GetWidth() const
    {
        return width;
    }

    uint32_t Backend::GetHeight() const
    {
        return height;
    }

#if defined(ORBIT_VULKAN_AVAILABLE)

    VkInstance Backend::GetInstance() const
    {
        return instance;
    }

    VkPhysicalDevice Backend::GetPhysicalDevice() const
    {
        return physicalDevice;
    }

    VkDevice Backend::GetDevice() const
    {
        return device;
    }

    VkQueue Backend::GetGraphicsQueue() const
    {
        return graphicsQueue;
    }

#endif
}
