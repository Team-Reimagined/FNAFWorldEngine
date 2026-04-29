#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace FWE::Renderer::Vulkan::Utils
{
    bool LoadShaderModule(const char *filePath, VkDevice device, VkShaderModule *outShaderModule);

    class PipelineBuilder
    {
    public:
        PipelineBuilder();

        void Clear();

        VkPipeline BuildPipeline(VkDevice device);

        void SetShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
        void SetInputTopology(VkPrimitiveTopology topology);
        void SetPolygonMode(VkPolygonMode mode);
        void SetCullMode(VkCullModeFlags cullMode, VkFrontFace frontFace);
        void SetMultisamplingNone();
        void DisableBlending();
        void SetColorAttachmentFormat(VkFormat format);
        void SetDepthFormat(VkFormat format);
        void DisableDepthTest();
    public:
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly;
        VkPipelineRasterizationStateCreateInfo rasterizer;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineMultisampleStateCreateInfo multisampling;
        VkPipelineLayout pipelineLayout;
        VkPipelineDepthStencilStateCreateInfo depthStencil;
        VkPipelineRenderingCreateInfo renderInfo;
        VkFormat colorAttachmentFormat;
    };
}