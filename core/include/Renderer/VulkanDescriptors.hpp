#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <span>

namespace FWE::Renderer::Vulkan
{
    struct DescriptorLayoutBuilder
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;

        void AddBinding(uint32_t binding, VkDescriptorType type);
        void Clear();
        VkDescriptorSetLayout Build(VkDevice device, VkShaderStageFlags shaderStages, void *pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);
    };
    
    struct DescriptorAllocator
    {
        struct PoolSizeRatio
        {
            VkDescriptorType type;
            float ratio;
        };

        VkDescriptorPool pool;

        void InitPool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
        void ClearDescriptors(VkDevice device);
        void DestroyPool(VkDevice device);

        VkDescriptorSet Allocate(VkDevice device, VkDescriptorSetLayout layout);
    };
}