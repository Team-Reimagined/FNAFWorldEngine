#include "Renderer/Vulkan.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "Renderer/VulkanInitalizers.hpp"
#include "Renderer/VulkanTypes.hpp"
#include "Renderer/VulkanImages.hpp"
#include "Renderer/VulkanPipelines.hpp"

#include <chrono>
#include <thread>

#include <VkBootstrap.h>

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_vulkan.h"

namespace FWE::Renderer::Vulkan
{
    constexpr bool useValidationLayers = false;

    Vulkan *Vulkan::GetInstance()
    {
        static Vulkan instance;
        return &instance;
    }

    FrameData &Vulkan::GetCurrentFrame()
    {
        return frames[frame % FRAME_OVERLAP];
    }

    void Vulkan::Init()
    {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(windowName, windowExtent.width, windowExtent.height, SDL_WINDOW_VULKAN);

        InitVulkan();
        InitSwapchain();
        InitCommands();
        InitSyncStructures();
        InitDescriptors();
        InitPipelines();
        InitImgui();

        initalized = true;
    }

    void Vulkan::InitVulkan()
    {
        vkb::InstanceBuilder builder;

        auto instanceReturn = builder.set_app_name(windowName).request_validation_layers(useValidationLayers).use_default_debug_messenger().require_api_version(1, 3, 0).build();

        vkb::Instance vkbInstance = instanceReturn.value();

        instance = vkbInstance.instance;
        debugMessenger = vkbInstance.debug_messenger;

        SDL_Vulkan_CreateSurface(window, instance, NULL, &surface);

        VkPhysicalDeviceVulkan13Features features13{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
        features13.dynamicRendering = true;
        features13.synchronization2 = true;

        VkPhysicalDeviceVulkan12Features features12{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
        features12.bufferDeviceAddress = true;
        features12.descriptorIndexing = true;

        vkb::PhysicalDeviceSelector selector{vkbInstance};
        vkb::PhysicalDevice physicalDevice = selector.set_minimum_version(1, 3).set_required_features_13(features13).set_required_features_12(features12).set_surface(surface).select().value();

        vkb::DeviceBuilder deviceBuilder{physicalDevice};
        vkb::Device vkbDevice = deviceBuilder.build().value();

        device = vkbDevice.device;
        gpu = physicalDevice.physical_device;

        graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
        graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = gpu;
        allocatorInfo.device = device;
        allocatorInfo.instance = instance;
        allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
        vmaCreateAllocator(&allocatorInfo, &allocator);

        mainDeletionQueue.PushFunction([&]()
        {
            vmaDestroyAllocator(allocator);
        });
    }

    void Vulkan::InitSwapchain()
    {
        CreateSwapchain(windowExtent.width, windowExtent.height);

        drawExtent = swapchainExtent;

        VkExtent3D drawImageExtent = {windowExtent.width, windowExtent.height, 1};

        drawImage.imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
        drawImage.imageExtent = drawImageExtent;

        VkImageUsageFlags drawImageUsages {};
        drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        drawImageUsages |= VK_IMAGE_USAGE_STORAGE_BIT;
        drawImageUsages |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        VkImageCreateInfo imgInfo = Utils::ImageCreateInfo(drawImage.imageFormat, drawImageUsages, drawImageExtent);

        VmaAllocationCreateInfo imgAllocInfo = {};
        imgAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        imgAllocInfo.requiredFlags =  VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        vmaCreateImage(allocator, &imgInfo, &imgAllocInfo, &drawImage.image, &drawImage.allocation, nullptr);

        VkImageViewCreateInfo viewInfo = Utils::ImageViewCreateInfo(drawImage.imageFormat, drawImage.image, VK_IMAGE_ASPECT_COLOR_BIT);

        VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &drawImage.imageView));

        mainDeletionQueue.PushFunction([=, this]()
        {
            vkDestroyImageView(device, drawImage.imageView, nullptr);
            vmaDestroyImage(allocator, drawImage.image, drawImage.allocation);
        });
    }

    void Vulkan::InitCommands()
    {
        VkCommandPoolCreateInfo commandPoolInfo = {};
        commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.pNext = nullptr;
        commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolInfo.queueFamilyIndex = graphicsQueueFamily;

        for(int i = 0; i < FRAME_OVERLAP; i++)
        {
            VK_CHECK(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &frames[i].commandPool));

            VkCommandBufferAllocateInfo cmdAllocInfo = Utils::CommandBufferAllocateInfo(frames[i].commandPool, 1);

            VK_CHECK(vkAllocateCommandBuffers(device, &cmdAllocInfo, &frames[i].mainCommandBuffer));
        }

        VK_CHECK(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &immCommandPool));

        VkCommandBufferAllocateInfo cmdAllocInfo = Utils::CommandBufferAllocateInfo(immCommandPool, 1);

        mainDeletionQueue.PushFunction([=, this]()
        {
            vkDestroyCommandPool(device, immCommandPool, nullptr);
        });
    }

    void Vulkan::InitSyncStructures()
    {
        VkFenceCreateInfo fenceCreateInfo = Utils::FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
        VkSemaphoreCreateInfo semaphoreCreateInfo = Utils::SemaphoreCreateInfo(0);

        for(int i = 0; i < FRAME_OVERLAP; i++)
        {
            VK_CHECK(vkCreateFence(device, &fenceCreateInfo, nullptr, &frames[i].renderFence));

            VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &frames[i].swapchainSemaphore));
            VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &frames[i].renderSemaphore));
        }

        VK_CHECK(vkCreateFence(device, &fenceCreateInfo, nullptr, &immFence));
        mainDeletionQueue.PushFunction([=, this]()
        {
            vkDestroyFence(device, immFence, nullptr);
        });
    }

    void Vulkan::InitDescriptors()
    {
        std::vector<DescriptorAllocator::PoolSizeRatio> sizes = {{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1}};

        globalDescriptorAllocator.InitPool(device, 10, sizes);

        {
            DescriptorLayoutBuilder builder;
            builder.AddBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
            drawImageDescriptorLayout = builder.Build(device, VK_SHADER_STAGE_COMPUTE_BIT);
        }

        drawImageDescriptors = globalDescriptorAllocator.Allocate(device, drawImageDescriptorLayout);

        VkDescriptorImageInfo imgInfo {};
        imgInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        imgInfo.imageView = drawImage.imageView;

        VkWriteDescriptorSet drawImageWrite = {};
        drawImageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        drawImageWrite.pNext = nullptr;

        drawImageWrite.dstBinding = 0;
        drawImageWrite.dstSet = drawImageDescriptors;
        drawImageWrite.descriptorCount = 1;
        drawImageWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        drawImageWrite.pImageInfo = &imgInfo;

        vkUpdateDescriptorSets(device, 1, &drawImageWrite, 0, nullptr);

        mainDeletionQueue.PushFunction([&]()
        {
            globalDescriptorAllocator.DestroyPool(device);

            vkDestroyDescriptorSetLayout(device, drawImageDescriptorLayout, nullptr);
        });
    }

    void Vulkan::InitPipelines()
    {
        InitBackgroundPipelines();
        InitTrianglePipeline();
    }

    void Vulkan::InitBackgroundPipelines()
    {
        VkPipelineLayoutCreateInfo computeLayout {};
        computeLayout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        computeLayout.pNext = nullptr;
        computeLayout.pSetLayouts = &drawImageDescriptorLayout;
        computeLayout.setLayoutCount = 1;

        VkPushConstantRange pushConstant{};
        pushConstant.offset = 0;
        pushConstant.size = sizeof(ComputePushConstants) ;
        pushConstant.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        computeLayout.pPushConstantRanges = &pushConstant;
        computeLayout.pushConstantRangeCount = 1;

        VK_CHECK(vkCreatePipelineLayout(device, &computeLayout, nullptr, &gradientPipelineLayout));

        VkShaderModule gradientShader;
        if(!Utils::LoadShaderModule("../shaders/gradient_color.comp.spv", device, &gradientShader))
        {
            Util::Logging::error("Error building compute shader");
        }

        VkShaderModule skyShader;
        if(!Utils::LoadShaderModule("../shaders/sky.comp.spv", device, &skyShader))
        {
            Util::Logging::error("Error building compute shader");
        }

        VkPipelineShaderStageCreateInfo stageInfo {};
        stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stageInfo.pNext = nullptr;
        stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        stageInfo.module = gradientShader;
        stageInfo.pName = "main";

        VkComputePipelineCreateInfo computePipelineCreateInfo {};
        computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        computePipelineCreateInfo.pNext = nullptr;
        computePipelineCreateInfo.layout = gradientPipelineLayout;
        computePipelineCreateInfo.stage = stageInfo;

        ComputeEffect gradient;
        gradient.layout = gradientPipelineLayout;
        gradient.name = "gradient";
        gradient.data = {};

        gradient.data.data1 = glm::vec4(1, 0, 0, 1);
        gradient.data.data2 = glm::vec4(0, 0, 1, 1);

        VK_CHECK(vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &gradient.pipeline));

        computePipelineCreateInfo.stage.module = skyShader;

        ComputeEffect sky;
        sky.layout  = gradientPipelineLayout;
        sky.name = "sky";
        sky.data = {};

        sky.data.data1 = glm::vec4(0.1, 0.2, 0.3, 0.97);

        VK_CHECK(vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &sky.pipeline));

        backgroundEffects.push_back(gradient);
        backgroundEffects.push_back(sky);

        vkDestroyShaderModule(device, gradientShader, nullptr);
        vkDestroyShaderModule(device, skyShader, nullptr);

        mainDeletionQueue.PushFunction([&]()
        {
            vkDestroyPipelineLayout(device, gradientPipelineLayout, nullptr);
            vkDestroyPipeline(device, gradient.pipeline, nullptr);
            vkDestroyPipeline(device, sky.pipeline, nullptr);
        });
    }

    void Vulkan::InitTrianglePipeline()
    {
        VkShaderModule triangleFragShader;
        if(!Utils::LoadShaderModule("../shaders/colored_triangle.frag.spv", device, &triangleFragShader))
        {
            Util::Logging::error("Error building fragment shader");
        }
        
        VkShaderModule triangleVertShader;
        if(!Utils::LoadShaderModule("../shaders/colored_triangle.vert.spv", device, &triangleVertShader))
        {
            Util::Logging::error("Error building vertex shader");
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = Utils::PipelineLayoutCreateInfo();
        VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &trianglePipelineLayout));

        Utils::PipelineBuilder pipelineBuilder;

        pipelineBuilder.pipelineLayout = trianglePipelineLayout;

        pipelineBuilder.SetShaders(triangleVertShader, triangleFragShader);
        pipelineBuilder.SetInputTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
        pipelineBuilder.SetPolygonMode(VK_POLYGON_MODE_FILL);
        pipelineBuilder.SetCullMode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
        pipelineBuilder.SetMultisamplingNone();
        pipelineBuilder.DisableBlending();
        pipelineBuilder.DisableDepthTest();

        pipelineBuilder.SetColorAttachmentFormat(drawImage.imageFormat);
        pipelineBuilder.SetDepthFormat(VK_FORMAT_UNDEFINED);

        trianglePipeline = pipelineBuilder.BuildPipeline(device);

        vkDestroyShaderModule(device, triangleFragShader, nullptr);
        vkDestroyShaderModule(device, triangleVertShader, nullptr);

        mainDeletionQueue.PushFunction([&]()
        {
            vkDestroyPipelineLayout(device, trianglePipelineLayout, nullptr);
            vkDestroyPipeline(device, trianglePipeline, nullptr);
        });
    }

    void Vulkan::InitImgui()
    {
        VkDescriptorPoolSize poolSizes[] =
        {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
        };

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000;
        poolInfo.poolSizeCount = (uint32_t)std::size(poolSizes);
        poolInfo.pPoolSizes = poolSizes;

        VkDescriptorPool imguiPool;
        VK_CHECK(vkCreateDescriptorPool(device, &poolInfo, nullptr, &imguiPool));

        ImGui::CreateContext();

        ImGui_ImplSDL3_InitForVulkan(window);

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = instance;
        initInfo.PhysicalDevice = gpu;
        initInfo.Device = device;
        initInfo.Queue = graphicsQueue;
        initInfo.DescriptorPool = imguiPool;
        initInfo.MinImageCount = 3;
        initInfo.ImageCount = 3;
        initInfo.UseDynamicRendering = true;

        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
        initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.pColorAttachmentFormats = &swapchainImageFormat;

        initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&initInfo);

        mainDeletionQueue.PushFunction([=, this]()
        {
            ImGui_ImplVulkan_Shutdown();
            vkDestroyDescriptorPool(device, imguiPool, nullptr);
        });
    }

    void Vulkan::ImmediateSubmit(std::function<void(VkCommandBuffer cmd)> &&function)
    {
        VK_CHECK(vkResetFences(device, 1, &immFence));
        VK_CHECK(vkResetCommandBuffer(immCommandBuffer, 0));

        VkCommandBuffer cmd = immCommandBuffer;

        VkCommandBufferBeginInfo cmdBeginInfo = Utils::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

        function(cmd);

        VK_CHECK(vkEndCommandBuffer(cmd));

        VkCommandBufferSubmitInfo cmdInfo = Utils::CommandBufferSubmitInfo(cmd);
        VkSubmitInfo2 submit = Utils::SubmitInfo(&cmdInfo, nullptr, nullptr);

        VK_CHECK(vkQueueSubmit2(graphicsQueue, 1, &submit, immFence));

        VK_CHECK(vkWaitForFences(device, 1, &immFence, true, 9999999999));
    }

    void Vulkan::Render()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if(ImGui::Begin("Background"))
        {
            ComputeEffect &selected = backgroundEffects[currentBackgroundEffect];

            ImGui::Text("Selected Effect: ", selected.name);

            ImGui::SliderInt("Effect Index", &currentBackgroundEffect, 0, backgroundEffects.size() - 1);

            ImGui::InputFloat4("data1", (float*)&selected.data.data1);
            ImGui::InputFloat4("data2", (float*)&selected.data.data2);
            ImGui::InputFloat4("data3", (float*)&selected.data.data3);
            ImGui::InputFloat4("data4", (float*)&selected.data.data4);
        }

        ImGui::End();

        ImGui::Render();

        VK_CHECK(vkWaitForFences(device, 1, &GetCurrentFrame().renderFence, true, 1000000000));

        GetCurrentFrame().deletionQueue.Flush();

        VK_CHECK(vkResetFences(device, 1, &GetCurrentFrame().renderFence));

        uint32_t swapchainImageIndex;
        VK_CHECK(vkAcquireNextImageKHR(device, swapchain, 1000000000, GetCurrentFrame().swapchainSemaphore, nullptr, &swapchainImageIndex));

        VkCommandBuffer cmd = GetCurrentFrame().mainCommandBuffer;

        VK_CHECK(vkResetCommandBuffer(cmd, 0));

        VkCommandBufferBeginInfo cmdBeginInfo = Utils::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

        Utils::TransitionImage(cmd, drawImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

        DrawBackground(cmd);

        Utils::TransitionImage(cmd, drawImage.image, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        DrawGeometry(cmd);

        Utils::TransitionImage(cmd, drawImage.image, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        Utils::TransitionImage(cmd, swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        Utils::CopyImageToImage(cmd, drawImage.image, swapchainImages[swapchainImageIndex], drawExtent, swapchainExtent);

        Utils::TransitionImage(cmd, swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        DrawImgui(cmd, swapchainImageViews[swapchainImageIndex]);

        Utils::TransitionImage(cmd, swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        VK_CHECK(vkEndCommandBuffer(cmd));

        VkCommandBufferSubmitInfo cmdInfo = Utils::CommandBufferSubmitInfo(cmd);

        VkSemaphoreSubmitInfo waitInfo = Utils::SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, GetCurrentFrame().swapchainSemaphore);
        VkSemaphoreSubmitInfo signalInfo = Utils::SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, GetCurrentFrame().renderSemaphore);

        VkSubmitInfo2 submit = Utils::SubmitInfo(&cmdInfo, &signalInfo, &waitInfo);

        VK_CHECK(vkQueueSubmit2(graphicsQueue, 1, &submit, GetCurrentFrame().renderFence));

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType =  VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.pSwapchains =  &swapchain;
        presentInfo.swapchainCount = 1;

        presentInfo.pWaitSemaphores = &GetCurrentFrame().renderSemaphore;
        presentInfo.waitSemaphoreCount = 1;

        presentInfo.pImageIndices = &swapchainImageIndex;

        VK_CHECK(vkQueuePresentKHR(graphicsQueue, &presentInfo));

        frame++;
    }

    void Vulkan::CreateSwapchain(uint32_t width, uint32_t height)
    {
        vkb::SwapchainBuilder swapchainBuilder{gpu, device, surface};

        swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

        vkb::Swapchain vkbSwapchain = swapchainBuilder.set_desired_format(VkSurfaceFormatKHR{.format = swapchainImageFormat,.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}).set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR).set_desired_extent(width, height).add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT).build().value();

        swapchainExtent = vkbSwapchain.extent;

        swapchain = vkbSwapchain.swapchain;
        swapchainImages = vkbSwapchain.get_images().value();
        swapchainImageViews = vkbSwapchain.get_image_views().value();
    }



    void Vulkan::DrawBackground(VkCommandBuffer cmd)
    {
        // VkClearColorValue clearValue;
        // float flash = std::abs(std::sin(frame / 120.f));
        // clearValue = {{0, 0, flash, 1}};

        // VkImageSubresourceRange clearRange = Utils::ImageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT);

        // vkCmdClearColorImage(cmd, drawImage.image, VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);

        ComputeEffect effect = backgroundEffects[currentBackgroundEffect];

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, effect.pipeline);

        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, gradientPipelineLayout, 0, 1, &drawImageDescriptors, 0, nullptr);

        vkCmdPushConstants(cmd, gradientPipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(ComputePushConstants), &effect.data);
        
        vkCmdDispatch(cmd, std::ceil(drawExtent.width / 16.0), std::ceil(drawExtent.height / 16.0), 1);
    }

    void Vulkan::DrawGeometry(VkCommandBuffer cmd)
    {
        VkRenderingAttachmentInfo colorAttachment = Utils::AttachmentInfo(drawImage.imageView, nullptr, VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL);

        VkRenderingInfo renderInfo = Utils::RenderingInfo(drawExtent, &colorAttachment, nullptr);
        vkCmdBeginRendering(cmd, &renderInfo);

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, trianglePipeline);

        VkViewport viewport = {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = drawExtent.width;
        viewport.height = drawExtent.height;
        viewport.minDepth = 0.f;
        viewport.maxDepth = 1.f;

        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent.width = drawExtent.width;
        scissor.extent.height = drawExtent.height;

        vkCmdSetScissor(cmd, 0, 1, &scissor);

        vkCmdDraw(cmd, 3, 1, 0, 0);

        vkCmdEndRendering(cmd);
    }

    void Vulkan::DrawImgui(VkCommandBuffer cmd, VkImageView targetImageView)
    {
        VkRenderingAttachmentInfo colorAttachment = Utils::AttachmentInfo(targetImageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        VkRenderingInfo renderInfo = Utils::RenderingInfo(swapchainExtent, &colorAttachment, nullptr);

        vkCmdBeginRendering(cmd, &renderInfo);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

        vkCmdEndRendering(cmd);
    }

    void Vulkan::DestroySwapchain()
    {
        vkDestroySwapchainKHR(device, swapchain, nullptr);

        for(int i = 0; i < swapchainImageViews.size(); i++)
        {
            vkDestroyImageView(device, swapchainImageViews[i], nullptr);
        }
    }

    void Vulkan::Shutdown()
    {
        if(initalized)
        {
            vkDeviceWaitIdle(device);

            for(int i = 0; i < FRAME_OVERLAP; i++)
            {
                vkDestroyCommandPool(device, frames[i].commandPool, nullptr);

                vkDestroyFence(device, frames[i].renderFence, nullptr);
                vkDestroySemaphore(device, frames[i].renderSemaphore, nullptr);
                vkDestroySemaphore(device, frames[i].swapchainSemaphore, nullptr);

                frames[i].deletionQueue.Flush();
            }

            mainDeletionQueue.Flush();

            DestroySwapchain();

            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyDevice(device, nullptr);

            vkb::destroy_debug_utils_messenger(instance, debugMessenger);
            vkDestroyInstance(instance, nullptr);

            SDL_DestroyWindow(window);
        }
    }
}