#pragma once

#include "VulkanTypes.hpp"
#include <SDL3/SDL.h>
#include "VulkanDescriptors.hpp"
#include "Types/Atlas.hpp"
#include "ResourceLoader/ImageLoader.hpp"
#include <unordered_map>

namespace FWE::Renderer::Vulkan
{
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
        void Init(bool fixedResolution, bool fullscreen);
        void Shutdown();
        void Render();
        void Draw(const FWE::Types::Atlas &atlas, int x = 0, int y = 0, float scaleX = 1, float scaleY = 1, float tileX = 1, float tileY = 1);
        int AddImage(const ResourceLoader::ImageResource &image);
        static Vulkan *GetInstance();
        
    private:
        void InitVulkan();
        void InitSwapchain();
        void InitCommands();
        void InitSyncStructures();
        void InitDescriptors();
        void InitPipelines();
        void InitMeshPipeline();
        void InitDefaultData();

        FrameData &GetCurrentFrame();

        void CreateSwapchain(uint32_t width, uint32_t height);
        void DestroySwapchain();
        
        void InitImgui();
        void DrawImgui(VkCommandBuffer cmd, VkImageView targetImageView);

        AllocatedBuffer CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
        void DestroyBuffer(const AllocatedBuffer &buffer);

        GPUMeshBuffers UploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);
        
        void ResizeSwapchain();

        void StartFrame();

        void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)> &&function);

        AllocatedImage CreateImage(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped = false);
        AllocatedImage CreateImage(void *data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped = false);
        void DestroyImage(const AllocatedImage &img);

    private:
        bool initalized = false;
        
        int frame = 0;

        const char *windowName = "FNaF World Engine";

        const uint32_t cameraWidth = 800;
        const uint32_t cameraHeight = 480;

        VkExtent2D windowExtent = {cameraWidth, cameraHeight};
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

        VkFence immFence;
        VkCommandBuffer immCommandBuffer;
        VkCommandPool immCommandPool;
        
        DeletionQueue mainDeletionQueue;
        VmaAllocator allocator;
        AllocatedImage drawImage;
        VkExtent2D drawExtent;

        VkPipelineLayout meshPipelineLayout;
        VkPipeline meshPipeline;

        GPUMeshBuffers rectangle;

        AllocatedImage blackImage;
        std::unordered_map<int, AllocatedImage> images;

        VkSampler defaultSamplerLinear;
        VkSampler defaultSamplerNearest;

        VkDescriptorSetLayout singleImageDescriptorLayout;

        VkCommandBuffer cmd;

        uint32_t swapchainImageIndex;

        bool resizeRequested = false;

        double aspectRatio;

        bool fixedResolution = true;

        bool frameStarted = false;

        int imageId = 0;
    };
}