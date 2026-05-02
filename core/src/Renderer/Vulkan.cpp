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
#include <glm/packing.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

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
        window = SDL_CreateWindow(windowName, windowExtent.width, windowExtent.height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

        InitVulkan();
        InitSwapchain();
        InitCommands();
        InitSyncStructures();
        InitDescriptors();
        InitPipelines();
        InitDefaultData();
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
        aspectRatio = (double)windowExtent.width / (double)windowExtent.height;
        
        CreateSwapchain(windowExtent.width, windowExtent.height);

        //drawExtent = swapchainExtent;

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

        // mainDeletionQueue.PushFunction([=, this]()
        // {
        //     vkDestroyImageView(device, drawImage.imageView, nullptr);
        //     vmaDestroyImage(allocator, drawImage.image, drawImage.allocation);
        // });
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

        VK_CHECK(vkAllocateCommandBuffers(device, &cmdAllocInfo, &immCommandBuffer));

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

        {
            DescriptorLayoutBuilder builder;
            builder.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
            gpuSceneDataDescriptorLayout = builder.Build(device, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
        }

        {
            DescriptorLayoutBuilder builder;
            builder.AddBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
            singleImageDescriptorLayout = builder.Build(device, VK_SHADER_STAGE_FRAGMENT_BIT);
        }

        drawImageDescriptors = globalDescriptorAllocator.Allocate(device, drawImageDescriptorLayout);

        DescriptorWriter writer;
        writer.WriteImage(0, drawImage.imageView, VK_NULL_HANDLE, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);

        writer.UpdateSet(device, drawImageDescriptors);

        mainDeletionQueue.PushFunction([&]()
        {
            globalDescriptorAllocator.DestroyPool(device);

            vkDestroyDescriptorSetLayout(device, drawImageDescriptorLayout, nullptr);
        });

        for(int i = 0; i < FRAME_OVERLAP; i++)
        {
            std::vector<DescriptorAllocatorGrowable::PoolSizeRatio> frameSizes = 
            {
                {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4}
            };

            frames[i].frameDescriptors = DescriptorAllocatorGrowable {};
            frames[i].frameDescriptors.Init(device, 1000, frameSizes);
            
            mainDeletionQueue.PushFunction([&, i]()
            {
                frames[i].frameDescriptors.DestroyPools(device);
            });
        }
    }

    void Vulkan::InitPipelines()
    {
        InitBackgroundPipelines();
        InitTrianglePipeline();
        InitMeshPipeline();
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

    void Vulkan::InitMeshPipeline()
    {
        VkShaderModule triangleFragShader;
        if(!Utils::LoadShaderModule("../shaders/tex_image.frag.spv", device, &triangleFragShader))
        {
            Util::Logging::error("Error building fragment shader");
        }
        
        VkShaderModule triangleVertShader;
        if(!Utils::LoadShaderModule("../shaders/colored_triangle_mesh.vert.spv", device, &triangleVertShader))
        {
            Util::Logging::error("Error building vertex shader");
        }

        VkPushConstantRange bufferRange {};
        bufferRange.offset = 0;
        bufferRange.size = sizeof(GPUDrawPushConstants);
        bufferRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = Utils::PipelineLayoutCreateInfo();
        pipelineLayoutInfo.pPushConstantRanges = &bufferRange;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pSetLayouts = &singleImageDescriptorLayout;
        pipelineLayoutInfo.setLayoutCount = 1;

        VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &meshPipelineLayout));

        Utils::PipelineBuilder pipelineBuilder;

        pipelineBuilder.pipelineLayout = meshPipelineLayout;

        pipelineBuilder.SetShaders(triangleVertShader, triangleFragShader);
        pipelineBuilder.SetInputTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
        pipelineBuilder.SetPolygonMode(VK_POLYGON_MODE_FILL);
        pipelineBuilder.SetCullMode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
        pipelineBuilder.SetMultisamplingNone();
        pipelineBuilder.DisableBlending();
        pipelineBuilder.DisableDepthTest();

        pipelineBuilder.SetColorAttachmentFormat(drawImage.imageFormat);
        pipelineBuilder.SetDepthFormat(VK_FORMAT_UNDEFINED);

        pipelineBuilder.EnableBlendingAlpha();

        meshPipeline = pipelineBuilder.BuildPipeline(device);

        vkDestroyShaderModule(device, triangleFragShader, nullptr);
        vkDestroyShaderModule(device, triangleVertShader, nullptr);

        mainDeletionQueue.PushFunction([&]()
        {
            vkDestroyPipelineLayout(device, meshPipelineLayout, nullptr);
            vkDestroyPipeline(device, meshPipeline, nullptr);
        });
    }

    void Vulkan::InitDefaultData()
    {
        std::array<Vertex, 4> rectVertices;

        rectVertices[0].position = {0.5, -0.5, 0};
        rectVertices[0].uvX = 1.0f;
        rectVertices[0].uvY = 0.0f;
        rectVertices[1].position = {0.5, 0.5, 0};
        rectVertices[1].uvX = 1.0f;
        rectVertices[1].uvY = 1.0f;
        rectVertices[2].position = {-0.5, -0.5, 0};
        rectVertices[2].uvX = 0.0f;
        rectVertices[2].uvY = 0.0f;
        rectVertices[3].position = {-0.5, 0.5, 0};
        rectVertices[3].uvX = 0.0f;
        rectVertices[3].uvY = 1.0f;

        rectVertices[0].color = {1, 1, 1, 1};
        rectVertices[1].color = {1, 1, 1, 1};
        rectVertices[2].color = {1, 1, 1, 1};
        rectVertices[3].color = {1, 1, 1, 1};

        std::array<uint32_t, 6> rectIndices;

        rectIndices[0] = 0;
        rectIndices[1] = 1;
        rectIndices[2] = 2;
        rectIndices[3] = 2;
        rectIndices[4] = 1;
        rectIndices[5] = 3;

        rectangle = UploadMesh(rectIndices, rectVertices);

        mainDeletionQueue.PushFunction([&]()
        {
            DestroyBuffer(rectangle.indexBuffer);
            DestroyBuffer(rectangle.vertexBuffer);            
        });

        uint32_t white = glm::packUnorm4x8(glm::vec4(1, 1, 1, 1));
        whiteImage = CreateImage((void*)&white, VkExtent3D{ 1, 1, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);

        uint32_t grey = glm::packUnorm4x8(glm::vec4(0.66f, 0.66f, 0.66f, 1));
        greyImage = CreateImage((void*)&grey, VkExtent3D{ 1, 1, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);

        uint32_t black = glm::packUnorm4x8(glm::vec4(0, 0, 0, 1));
        blackImage = CreateImage((void*)&black, VkExtent3D{ 1, 1, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);

        uint32_t magenta = glm::packUnorm4x8(glm::vec4(1, 0, 1, 1));
        std::array<uint32_t, 16 * 16 > pixels;
        for (int x = 0; x < 16; x++)
        {
            for (int y = 0; y < 16; y++)
            {
                pixels[y*16 + x] = ((x % 2) ^ (y % 2)) ? magenta : black;
            }
        }
        errorCheckerboardImage = CreateImage(pixels.data(), VkExtent3D{16, 16, 1}, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);

        VkSamplerCreateInfo sample = {.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};

        sample.magFilter = VK_FILTER_NEAREST;
        sample.minFilter = VK_FILTER_NEAREST;

        vkCreateSampler(device, &sample, nullptr, &defaultSamplerNearest);

        sample.magFilter = VK_FILTER_LINEAR;
        sample.minFilter = VK_FILTER_LINEAR;
        vkCreateSampler(device, &sample, nullptr, &defaultSamplerLinear);

        mainDeletionQueue.PushFunction([&]()
        {
            vkDestroySampler(device, defaultSamplerNearest, nullptr);
            vkDestroySampler(device, defaultSamplerLinear, nullptr);

            DestroyImage(whiteImage);
            DestroyImage(greyImage);
            DestroyImage(blackImage);
            DestroyImage(errorCheckerboardImage);
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

    AllocatedBuffer Vulkan::CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
    {
        VkBufferCreateInfo bufferInfo = {.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferInfo.pNext = nullptr;
        bufferInfo.size = allocSize;

        bufferInfo.usage = usage;

        VmaAllocationCreateInfo vmaAllocInfo = {};
        vmaAllocInfo.usage = memoryUsage;
        vmaAllocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
        AllocatedBuffer newBuffer;

        VK_CHECK(vmaCreateBuffer(allocator, &bufferInfo, &vmaAllocInfo, &newBuffer.buffer, &newBuffer.allocation, &newBuffer.info));

        return newBuffer;
    }

    void Vulkan::DestroyBuffer(const AllocatedBuffer &buffer)
    {
        vmaDestroyBuffer(allocator, buffer.buffer, buffer.allocation);
    }

    GPUMeshBuffers Vulkan::UploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices)
    {
        const size_t vertexBufferSize = vertices.size() * sizeof(Vertex);
        const size_t indexBufferSize = indices.size() * sizeof(uint32_t);

        GPUMeshBuffers newSurface;

        newSurface.vertexBuffer = CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

        VkBufferDeviceAddressInfo deviceAddressInfo {.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO, .buffer = newSurface.vertexBuffer.buffer};
        newSurface.vertexBufferAddress = vkGetBufferDeviceAddress(device, &deviceAddressInfo);

        newSurface.indexBuffer = CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

        AllocatedBuffer staging = CreateBuffer(vertexBufferSize + indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);

        void *data = staging.allocation->GetMappedData();

        memcpy(data, vertices.data(), vertexBufferSize);

        memcpy((char *)data + vertexBufferSize, indices.data(), indexBufferSize);

        ImmediateSubmit([&](VkCommandBuffer cmd)
        {
            VkBufferCopy vertexCopy {0};
            vertexCopy.dstOffset = 0; 
            vertexCopy.srcOffset = 0;
            vertexCopy.size = vertexBufferSize;
            
            vkCmdCopyBuffer(cmd, staging.buffer, newSurface.vertexBuffer.buffer, 1, &vertexCopy);

            VkBufferCopy indexCopy {0};
            indexCopy.dstOffset = 0;
            indexCopy.srcOffset = vertexBufferSize;
            indexCopy.size = indexBufferSize;

            vkCmdCopyBuffer(cmd, staging.buffer, newSurface.indexBuffer.buffer, 1, &indexCopy);
        });

        DestroyBuffer(staging);

        return newSurface;
    }

    void Vulkan::Render()
    {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        
        if(resizeRequested || (w != swapchainExtent.width) || (h != swapchainExtent.height))
        {
            ResizeSwapchain();
        }
        drawExtent.height = drawImage.imageExtent.height;
        drawExtent.width = drawImage.imageExtent.width;

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

        GetCurrentFrame().deletionQueue.Flush();
        GetCurrentFrame().frameDescriptors.ClearPools(device);

        uint32_t swapchainImageIndex;

        VkResult result = vkAcquireNextImageKHR(device, swapchain, 1000000000, GetCurrentFrame().swapchainSemaphore, nullptr, &swapchainImageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            resizeRequested = true;
            return;
        }

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

        VkResult resultPresent = vkQueuePresentKHR(graphicsQueue, &presentInfo);
        if(resultPresent == VK_ERROR_OUT_OF_DATE_KHR)
        {
            resizeRequested = true;
        }

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

    void Vulkan::ResizeSwapchain()
    {
        vkDeviceWaitIdle(device);

        DestroySwapchain();

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        windowExtent.width = w;
        windowExtent.height = h;

        if(resizable)
        {
            vkDestroyImageView(device, drawImage.imageView, nullptr);
            vmaDestroyImage(allocator, drawImage.image, drawImage.allocation);
            InitSwapchain();
        }
        else
        {
            CreateSwapchain(windowExtent.width, windowExtent.height);
        }

        resizeRequested = false;
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

        // vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, trianglePipeline);

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

        // vkCmdDraw(cmd, 3, 1, 0, 0);

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, meshPipeline);

        // GPUDrawPushConstants pushConstants;
        // pushConstants.worldMatrix = glm::mat4 {1.f};
        // pushConstants.vertexBuffer = rectangle.vertexBufferAddress;

        // vkCmdPushConstants(cmd, meshPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GPUDrawPushConstants), &pushConstants);
        // vkCmdBindIndexBuffer(cmd, rectangle.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

        // vkCmdDrawIndexed(cmd, 6, 1, 0, 0, 0);

        VkDescriptorSet imageSet = GetCurrentFrame().frameDescriptors.Allocate(device, singleImageDescriptorLayout);
        {
            DescriptorWriter writer;
            writer.WriteImage(0, errorCheckerboardImage.imageView, defaultSamplerNearest, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

            writer.UpdateSet(device, imageSet);
        }

        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, meshPipelineLayout, 0, 1, &imageSet, 0, nullptr);

        glm::mat4 view = glm::translate(glm::vec3{ 0,0,-5 });
        glm::mat4 projection = glm::perspective(glm::radians(70.f), (float)drawExtent.width / (float)drawExtent.height, 10000.f, 0.1f);

        projection[1][1] *= -1;

        GPUDrawPushConstants push_constants;
        push_constants.worldMatrix = glm::mat4 {1.f};
        push_constants.vertexBuffer = rectangle.vertexBufferAddress;

        vkCmdPushConstants(cmd, meshPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GPUDrawPushConstants), &push_constants);
        vkCmdBindIndexBuffer(cmd, rectangle.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(cmd, 6, 1, 0, 0, 0);

        AllocatedBuffer gpuSceneDataBuffer = CreateBuffer(sizeof(GPUSceneData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        GetCurrentFrame().deletionQueue.PushFunction([=, this]()
        {
            DestroyBuffer(gpuSceneDataBuffer);  
        });

        GPUSceneData *sceneUniformData = (GPUSceneData*)gpuSceneDataBuffer.allocation->GetMappedData();
        *sceneUniformData = sceneData;

        VkDescriptorSet globalDescriptor = GetCurrentFrame().frameDescriptors.Allocate(device, gpuSceneDataDescriptorLayout);

        DescriptorWriter writer;
        writer.WriteBuffer(0, gpuSceneDataBuffer.buffer, sizeof(GPUSceneData), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        writer.UpdateSet(device, globalDescriptor);

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

            vkDestroyImageView(device, drawImage.imageView, nullptr);
            vmaDestroyImage(allocator, drawImage.image, drawImage.allocation);
            mainDeletionQueue.Flush();

            DestroySwapchain();

            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyDevice(device, nullptr);

            vkb::destroy_debug_utils_messenger(instance, debugMessenger);
            vkDestroyInstance(instance, nullptr);

            SDL_DestroyWindow(window);
        }
    }

    AllocatedImage Vulkan::CreateImage(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped)
    {
        AllocatedImage newImage;
        newImage.imageFormat = format;
        newImage.imageExtent = size;

        VkImageCreateInfo imgInfo = Utils::ImageCreateInfo(format, usage, size);
        if(mipmapped)
        {
            imgInfo.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(size.width, size.height)))) + 1;
        }

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        allocInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VK_CHECK(vmaCreateImage(allocator, &imgInfo, &allocInfo, &newImage.image, &newImage.allocation, nullptr));

        VkImageAspectFlags aspectFlag = VK_IMAGE_ASPECT_COLOR_BIT;
        if(format == VK_FORMAT_D32_SFLOAT)
        {
            aspectFlag = VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        VkImageViewCreateInfo viewInfo = Utils::ImageViewCreateInfo(format, newImage.image, aspectFlag);
        viewInfo.subresourceRange.levelCount = imgInfo.mipLevels;

        VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &newImage.imageView));

        return newImage;
    }

    AllocatedImage Vulkan::CreateImage(void *data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped)
    {
        size_t dataSize = size.depth * size.width * size.height * 4;
        AllocatedBuffer uploadBuffer = CreateBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        memcpy(uploadBuffer.info.pMappedData, data, dataSize);

        AllocatedImage newImage = CreateImage(size, format, usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, mipmapped);

        ImmediateSubmit([&](VkCommandBuffer cmd)
        {
            Utils::TransitionImage(cmd, newImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            VkBufferImageCopy copyRegion = {};
            copyRegion.bufferOffset = 0;
            copyRegion.bufferRowLength = 0;
            copyRegion.bufferImageHeight = 0;

            copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.imageSubresource.mipLevel = 0;
            copyRegion.imageSubresource.baseArrayLayer = 0;
            copyRegion.imageSubresource.layerCount = 1;
            copyRegion.imageExtent = size;

            vkCmdCopyBufferToImage(cmd, uploadBuffer.buffer, newImage.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

            Utils::TransitionImage(cmd, newImage.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        });

        DestroyBuffer(uploadBuffer);

        return newImage;
    }

    void Vulkan::DestroyImage(const AllocatedImage &img)
    {
        vkDestroyImageView(device, img.imageView, nullptr);
        vmaDestroyImage(allocator, img.image, img.allocation);
    }
}