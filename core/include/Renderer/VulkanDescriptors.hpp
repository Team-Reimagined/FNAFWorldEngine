#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <span>
#include <deque>

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

    struct DescriptorAllocatorGrowable
    {
        struct PoolSizeRatio
        {
            VkDescriptorType type;
            float ratio;
        };

        void Init(VkDevice device, uint32_t initailSets, std::span<PoolSizeRatio> poolRatios);
        void ClearPools(VkDevice device);
        void DestroyPools(VkDevice device);

        VkDescriptorSet Allocate(VkDevice device, VkDescriptorSetLayout layout, void *pNext = nullptr);

    private:
        VkDescriptorPool GetPool(VkDevice device);
        VkDescriptorPool CreatePool(VkDevice device, uint32_t setCount, std::span<PoolSizeRatio> poolRatios);

        std::vector<PoolSizeRatio> ratios;
        std::vector<VkDescriptorPool> fullPools;
        std::vector<VkDescriptorPool> readyPools;
        uint32_t setsPerPool;
    };

    struct DescriptorWriter
    {
        std::deque<VkDescriptorImageInfo> imageInfos;
        std::deque<VkDescriptorBufferInfo> bufferInfos;
        std::vector<VkWriteDescriptorSet> writes;

        void WriteImage(int binding, VkImageView image, VkSampler sammpler, VkImageLayout layout, VkDescriptorType type);
        void WriteBuffer(int binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type);

        void Clear();
        void UpdateSet(VkDevice device, VkDescriptorSet set);
    };
}