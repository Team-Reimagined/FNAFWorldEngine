#pragma once

#include "VulkanTypes.hpp"
#include <SDL3/SDL.h>

namespace FWE::Renderer
{
    struct FrameData
    {
        VkCommandPool commandPool;
        VkCommandBuffer mainCommandBuffer;
    };

    constexpr unsigned int FRAME_OVERLAP = 2;
    
    class Vulkan
    {
    public:
        void Init();
        void Shutdown();
        void Draw();
        void Run();
        static Vulkan *GetInstance();
        FrameData &GetCurrentFrame();
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
        
    private:
        void InitVulkan();
        void InitSwapchain();
        void InitCommands();
        void InitSyncStructures();

        void CreateSwapchain(uint32_t width, uint32_t height);
        void DestroySwapchain();
    private:
        const char *windowName = "FNaF World Engine";
    };
}