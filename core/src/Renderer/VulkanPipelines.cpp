#include <Renderer/VulkanPipelines.hpp>
#include <fstream>
#include <Renderer/VulkanInitalizers.hpp>
#include "Util/Logging.hpp"

namespace FWE::Renderer::Vulkan::Utils
{
    bool LoadShaderModule(const char *filePath, VkDevice device, VkShaderModule *outShaderModule)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if(!file.is_open())
        {
            return false;
        }

        size_t fileSize = (size_t)file.tellg();

        std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

        file.seekg(0);

        file.read((char*)buffer.data(), fileSize);

        file.close();

        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = nullptr;

        createInfo.codeSize = buffer.size() * sizeof(uint32_t);
        createInfo.pCode = buffer.data();

        VkShaderModule shaderModule;
        if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            return false;
        }

        *outShaderModule = shaderModule;
        return true;
    }
    
    PipelineBuilder::PipelineBuilder()
    {
        Clear();
    }

    void PipelineBuilder::Clear()
    {
        inputAssembly = {.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};

        rasterizer = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};

        colorBlendAttachment = {};

        multisampling = {.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};

        pipelineLayout = {};

        depthStencil = {.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};

        renderInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};

        shaderStages.clear();
    }

    VkPipeline PipelineBuilder::BuildPipeline(VkDevice device)
    {
        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pNext = nullptr;

        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.pNext = nullptr;

        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};

        VkGraphicsPipelineCreateInfo pipelineInfo = {.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};

        pipelineInfo.pNext = &renderInfo;

        pipelineInfo.stageCount = (uint32_t)shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.layout = pipelineLayout;

        VkDynamicState state[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamicInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
        dynamicInfo.pDynamicStates = &state[0];
        dynamicInfo.dynamicStateCount = 2;
        
        pipelineInfo.pDynamicState = &dynamicInfo;

        VkPipeline newPipeline;
        if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS)
        {
            FWE::Util::Logging::error("Failed to create pipeline");
            return VK_NULL_HANDLE;
        }
        else
        {
            return newPipeline;
        }
    }

    void PipelineBuilder::SetShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader)
    {
        shaderStages.clear();

        shaderStages.push_back(Utils::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShader));

        shaderStages.push_back(Utils::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader));
    }

    void PipelineBuilder::SetInputTopology(VkPrimitiveTopology topology)
    {
        inputAssembly.topology = topology;

        inputAssembly.primitiveRestartEnable = VK_FALSE;
    }

    void PipelineBuilder::SetPolygonMode(VkPolygonMode mode)
    {
        rasterizer.polygonMode = mode;
        rasterizer.lineWidth = 1.f;
    }

    void PipelineBuilder::SetCullMode(VkCullModeFlags cullMode, VkFrontFace frontFace)
    {
        rasterizer.cullMode = cullMode;
        rasterizer.frontFace = frontFace;
    }

    void PipelineBuilder::SetMultisamplingNone()
    {
        multisampling.sampleShadingEnable = VK_FALSE;

        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.f;
        multisampling.pSampleMask = nullptr;

        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;
    }

    void PipelineBuilder::DisableBlending()
    {
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        colorBlendAttachment.blendEnable = VK_FALSE;
    }

    void PipelineBuilder::SetColorAttachmentFormat(VkFormat format)
    {
        colorAttachmentFormat = format;

        renderInfo.colorAttachmentCount = 1;
        renderInfo.pColorAttachmentFormats = &colorAttachmentFormat;
    }

    void PipelineBuilder::SetDepthFormat(VkFormat format)
    {
        renderInfo.depthAttachmentFormat = format;
    }

    void PipelineBuilder::DisableDepthTest()
    {
        depthStencil.depthTestEnable = VK_FALSE;
        depthStencil.depthWriteEnable = VK_FALSE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_NEVER;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};
        depthStencil.minDepthBounds = 0.f;
        depthStencil.maxDepthBounds = 1.f;
    }
}