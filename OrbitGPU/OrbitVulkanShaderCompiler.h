#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

class OrbitVulkanShaderCompiler
{
public:
    static bool CompileGLSL(
        const std::string& source,
        VkShaderStageFlagBits stage,
        std::vector<uint32_t>& spirv,
        std::string& error
    );

    static bool CompileFile(
        const std::string& path,
        VkShaderStageFlagBits stage,
        std::vector<uint32_t>& spirv,
        std::string& error
    );

private:
    static std::string StageName(
        VkShaderStageFlagBits stage
    );
};
