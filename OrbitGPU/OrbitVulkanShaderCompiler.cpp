#include "OrbitVulkanShaderCompiler.h"

#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

std::string OrbitVulkanShaderCompiler::StageName(
    VkShaderStageFlagBits stage)
{
    switch (stage)
    {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return "vert";

        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return "frag";

        case VK_SHADER_STAGE_COMPUTE_BIT:
            return "comp";

        default:
            return "";
    }
}

bool OrbitVulkanShaderCompiler::CompileGLSL(
    const std::string& source,
    VkShaderStageFlagBits stage,
    std::vector<uint32_t>& spirv,
    std::string& error)
{
    spirv.clear();
    error.clear();

    const std::string stageName = StageName(stage);

    if (stageName.empty())
    {
        error = "Unsupported shader stage";
        return false;
    }

    const std::string base =
        "/data/data/com.termux/files/usr/tmp/orbit_shader";

    const std::string input =
        base + "." + stageName + ".glsl";

    const std::string output =
        base + "." + stageName + ".spv";

    {
        std::ofstream file(input);

        if (!file)
        {
            error = "Unable to create shader source file";
            return false;
        }

        file << source;
    }

    const std::string command =
        "glslc -fshader-stage=" +
        stageName +
        " \"" + input +
        "\" -o \"" + output +
        "\" 2>/tmp/orbit_shader_error.txt";

    const int result = std::system(command.c_str());

    if (result != 0)
    {
        std::ifstream errorFile(
            "/tmp/orbit_shader_error.txt");

        std::stringstream buffer;
        buffer << errorFile.rdbuf();

        error = buffer.str();

        if (error.empty())
            error = "glslc compilation failed";

        std::remove(input.c_str());
        std::remove(output.c_str());

        return false;
    }

    std::ifstream file(
        output,
        std::ios::binary | std::ios::ate
    );

    if (!file)
    {
        error = "Compiled SPIR-V file not found";

        std::remove(input.c_str());
        std::remove(output.c_str());

        return false;
    }

    const std::streamsize byteSize =
        file.tellg();

    file.seekg(0);

    if (byteSize <= 0 ||
        byteSize % sizeof(uint32_t) != 0)
    {
        error = "Invalid SPIR-V binary";

        std::remove(input.c_str());
        std::remove(output.c_str());

        return false;
    }

    spirv.resize(
        static_cast<size_t>(
            byteSize / sizeof(uint32_t)));

    file.read(
        reinterpret_cast<char*>(spirv.data()),
        byteSize
    );

    file.close();

    std::remove(input.c_str());
    std::remove(output.c_str());

    return true;
}

bool OrbitVulkanShaderCompiler::CompileFile(
    const std::string& path,
    VkShaderStageFlagBits stage,
    std::vector<uint32_t>& spirv,
    std::string& error)
{
    std::ifstream file(path);

    if (!file)
    {
        error = "Unable to open shader file: " + path;
        return false;
    }

    std::stringstream source;
    source << file.rdbuf();

    return CompileGLSL(
        source.str(),
        stage,
        spirv,
        error
    );
}
