#pragma once

#include "VulkanTypes.hpp"
#include <SDL3/SDL.h>
#include "VulkanDescriptors.hpp"

namespace FWE::Renderer::Vulkan
{
    struct GPUSceneData
    {
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 viewproj;
        glm::vec4 ambientColor;
        glm::vec4 sunlightDirection;
        glm::vec4 sunlightColor;
    };

    struct AllocatedImage
    {
        VkImage image;
        VkImageView imageView;
        VmaAllocation allocation;
        VkExtent3D imageExtent;
        VkFormat imageFormat;  
    };

    struct DeletionQueue
    {
        std::deque<std::function<void()>> deletors;
        
        void PushFunction(std::function<void()> &&function)
        {
            deletors.push_back(function);
        }

        void Flush()
        {
            for(auto it = deletors.rbegin(); it != deletors.rend(); it++)
            {
                (*it)();
            }

            deletors.clear();
        }
    };

    struct ComputePushConstants
    {
        glm::vec4 data1;
        glm::vec4 data2;
        glm::vec4 data3;
        glm::vec4 data4;
    };

    struct ComputeEffect
    {
        const char *name;

        VkPipeline pipeline;
        VkPipelineLayout layout;

        ComputePushConstants data;
    };

    struct FrameData
    {
        VkCommandPool commandPool;
        VkCommandBuffer mainCommandBuffer;

        VkSemaphore swapchainSemaphore, renderSemaphore;
        VkFence renderFence;

        DeletionQueue deletionQueue;
        DescriptorAllocatorGrowable frameDescriptors;
    };

    constexpr unsigned int FRAME_OVERLAP = 2;
    
    class Vulkan
    {
    public:
        void Init();
        void Shutdown();
        void Render();
        void Run();
        static Vulkan *GetInstance();
        FrameData &GetCurrentFrame();

        void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)> &&function);

        AllocatedImage CreateImage(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped = false);
        AllocatedImage CreateImage(void *data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped = false);
        void DestroyImage(const AllocatedImage &img);
    public:
        bool initalized = false;
        int frame = 0;
        bool stopRendering = false;
        VkExtent2D windowExtent = {800, 480};
        SDL_Window *window = nullptr;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice gpu;
        VkDevice device;
        VkSurfaceKHR surface;

        VkSwapchainKHR swapchain;
        VkFormat swapchainImageFormat;

        std::vector<VkImage> swapchainImages;
        std::vector<VkImageView> swapchainImageViews;
        VkExtent2D swapchainExtent;
        
        FrameData frames[FRAME_OVERLAP];
        
        VkQueue graphicsQueue;
        uint32_t graphicsQueueFamily;

        DescriptorAllocator globalDescriptorAllocator;

        VkDescriptorSet drawImageDescriptors;
        VkDescriptorSetLayout drawImageDescriptorLayout;

        VkPipelineLayout gradientPipelineLayout;

        VkFence immFence;
        VkCommandBuffer immCommandBuffer;
        VkCommandPool immCommandPool;
    private:
        void InitVulkan();
        void InitSwapchain();
        void InitCommands();
        void InitSyncStructures();
        void InitDescriptors();
        void InitPipelines();
        void InitBackgroundPipelines();
        void InitTrianglePipeline();
        void InitMeshPipeline();
        void InitDefaultData();

        void CreateSwapchain(uint32_t width, uint32_t height);
        void DestroySwapchain();

        void DrawBackground(VkCommandBuffer cmd);
        void DrawGeometry(VkCommandBuffer cmd);
        
        void InitImgui();
        void DrawImgui(VkCommandBuffer cmd, VkImageView targetImageView);

        AllocatedBuffer CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
        void DestroyBuffer(const AllocatedBuffer &buffer);

        GPUMeshBuffers UploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);
        
        void ResizeSwapchain();

    private:
        const char *windowName = "FNaF World Engine";
        DeletionQueue mainDeletionQueue;
        VmaAllocator allocator;
        AllocatedImage drawImage;
        VkExtent2D drawExtent;
        std::vector<ComputeEffect> backgroundEffects;
        int currentBackgroundEffect {0};

        VkPipelineLayout trianglePipelineLayout;
        VkPipeline trianglePipeline;

        VkPipelineLayout meshPipelineLayout;
        VkPipeline meshPipeline;

        GPUMeshBuffers rectangle;

        bool resizeRequested = false;

        double aspectRatio;

        bool resizable = false;

        GPUSceneData sceneData;

        VkDescriptorSetLayout gpuSceneDataDescriptorLayout;

        AllocatedImage whiteImage;
        AllocatedImage blackImage;
        AllocatedImage greyImage;
        AllocatedImage errorCheckerboardImage;

        VkSampler defaultSamplerLinear;
        VkSampler defaultSamplerNearest;

        VkDescriptorSetLayout singleImageDescriptorLayout;
    };
}