#pragma once

#include "VulkanTypes.hpp"
#include <SDL3/SDL.h>
#include "VulkanDescriptors.hpp"

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

        VkPipeline gradientPipeline;
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

        void CreateSwapchain(uint32_t width, uint32_t height);
        void DestroySwapchain();

        void DrawBackground(VkCommandBuffer cmd);
        
        void InitImgui();
        void DrawImgui(VkCommandBuffer cmd, VkImageView targetImageView);
    private:
        const char *windowName = "FNaF World Engine";
        DeletionQueue mainDeletionQueue;
        VmaAllocator allocator;
        AllocatedImage drawImage;
        VkExtent2D drawExtent;
        std::vector<ComputeEffect> backgroundEffects;
        int currentBackgroundEffect {0};
    };
}