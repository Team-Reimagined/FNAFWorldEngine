#pragma once

#include <vulkan/vulkan.h>

namespace FWE::Renderer::Vulkan::Utils
{
    bool LoadShaderModule(const char *filePath, VkDevice device, VkShaderModule *outShaderModule);
};