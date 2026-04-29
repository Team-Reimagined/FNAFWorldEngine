#pragma once

#include <vulkan/vulkan.h>

namespace FWE::Renderer::Vulkan::Utils
{
    VkImageSubresourceRange ImageSubresourceRange(VkImageAspectFlags aspectMask);

    void TransitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);

    void CopyImageToImage(VkCommandBuffer cmd, VkImage source, VkImage destination, VkExtent2D sourceSize, VkExtent2D destinationSize);
}