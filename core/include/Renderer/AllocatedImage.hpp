#pragma once

#include "Renderer/VulkanTypes.hpp"
#include <vulkan/vulkan_core.h>

namespace FWE::Renderer
{
    struct AllocatedImage
    {
        VkImage image = nullptr;
        VkImageView imageView = nullptr;
        VmaAllocation allocation = nullptr;
        VkExtent3D imageExtent;
        VkFormat imageFormat;  
    };
}