#include "OrbitVulkanGraphicsPipeline.h"
#include "OrbitVulkanVertexInput.h"

OrbitVulkanGraphicsPipeline::
OrbitVulkanGraphicsPipeline()
    : device_(VK_NULL_HANDLE),
      pipeline_(VK_NULL_HANDLE)
{
}

OrbitVulkanGraphicsPipeline::
~OrbitVulkanGraphicsPipeline()
{
    Destroy();
}

bool OrbitVulkanGraphicsPipeline::Create(
    VkDevice device,
    VkPipelineCache cache,
    VkPipelineLayout layout,
    VkRenderPass renderPass,
    VkShaderModule vertexShader,
    VkShaderModule fragmentShader,
    VkExtent2D extent)
{
    if (device == VK_NULL_HANDLE ||
        layout == VK_NULL_HANDLE ||
        renderPass == VK_NULL_HANDLE ||
        vertexShader == VK_NULL_HANDLE ||
        fragmentShader == VK_NULL_HANDLE)
    {
        return false;
    }

    Destroy();

    VkPipelineShaderStageCreateInfo stages[2]{};

    stages[0].sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

    stages[0].stage =
        VK_SHADER_STAGE_VERTEX_BIT;

    stages[0].module =
        vertexShader;

    stages[0].pName = "main";

    stages[1].sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

    stages[1].stage =
        VK_SHADER_STAGE_FRAGMENT_BIT;

    stages[1].module =
        fragmentShader;

    stages[1].pName = "main";

    VkVertexInputBindingDescription binding =
        OrbitVulkanVertexInput::Binding();

    std::vector<VkVertexInputAttributeDescription>
        attributes =
        OrbitVulkanVertexInput::Attributes();

    VkPipelineVertexInputStateCreateInfo vertexInput{};

    vertexInput.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    vertexInput.vertexBindingDescriptionCount = 1;
    vertexInput.pVertexBindingDescriptions = &binding;

    vertexInput.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(attributes.size());

    vertexInput.pVertexAttributeDescriptions =
        attributes.data();

    VkPipelineInputAssemblyStateCreateInfo assembly{};

    assembly.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

    assembly.topology =
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    assembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};

    viewport.x = 0.0f;
    viewport.y = 0.0f;

    viewport.width =
        static_cast<float>(extent.width);

    viewport.height =
        static_cast<float>(extent.height);

    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};

    scissor.offset = {0, 0};
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewportState{};

    viewportState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;

    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo raster{};

    raster.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

    raster.depthClampEnable = VK_FALSE;
    raster.rasterizerDiscardEnable = VK_FALSE;

    raster.polygonMode =
        VK_POLYGON_MODE_FILL;

    raster.cullMode =
        VK_CULL_MODE_BACK_BIT;

    raster.frontFace =
        VK_FRONT_FACE_COUNTER_CLOCKWISE;

    raster.depthBiasEnable = VK_FALSE;

    raster.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisample{};

    multisample.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

    multisample.rasterizationSamples =
        VK_SAMPLE_COUNT_1_BIT;

    multisample.sampleShadingEnable =
        VK_FALSE;

    VkPipelineColorBlendAttachmentState blendAttachment{};

    blendAttachment.blendEnable = VK_FALSE;

    blendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo blend{};

    blend.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

    blend.logicOpEnable = VK_FALSE;

    blend.attachmentCount = 1;
    blend.pAttachments = &blendAttachment;

    VkPipelineDepthStencilStateCreateInfo depth{};

    depth.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    depth.depthTestEnable = VK_TRUE;
    depth.depthWriteEnable = VK_TRUE;

    depth.depthCompareOp =
        VK_COMPARE_OP_LESS;

    depth.depthBoundsTestEnable = VK_FALSE;
    depth.stencilTestEnable = VK_FALSE;

    VkGraphicsPipelineCreateInfo info{};

    info.sType =
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    info.stageCount = 2;
    info.pStages = stages;

    info.pVertexInputState = &vertexInput;
    info.pInputAssemblyState = &assembly;
    info.pViewportState = &viewportState;
    info.pRasterizationState = &raster;
    info.pMultisampleState = &multisample;
    info.pDepthStencilState = &depth;
    info.pColorBlendState = &blend;

    info.layout = layout;
    info.renderPass = renderPass;
    info.subpass = 0;

    if (vkCreateGraphicsPipelines(
            device,
            cache,
            1,
            &info,
            nullptr,
            &pipeline_) != VK_SUCCESS)
    {
        pipeline_ = VK_NULL_HANDLE;
        return false;
    }

    device_ = device;

    return true;
}

void OrbitVulkanGraphicsPipeline::Destroy()
{
    if (device_ != VK_NULL_HANDLE &&
        pipeline_ != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(
            device_,
            pipeline_,
            nullptr
        );
    }

    pipeline_ = VK_NULL_HANDLE;
    device_ = VK_NULL_HANDLE;
}

VkPipeline OrbitVulkanGraphicsPipeline::Get() const
{
    return pipeline_;
}

bool OrbitVulkanGraphicsPipeline::IsValid() const
{
    return pipeline_ != VK_NULL_HANDLE;
}
