#pragma once
#include "Render/RenderDevice.h"
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"

#include"vulkan/vulkan_win32.h"

#include<optional>
#include<mutex>




namespace PRE
{

	struct QueueFamilyIndices 
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
	    
		bool isComplete()
		{
			return graphicsFamily.has_value()&&presentFamily.has_value();
		}
	};


	class RenderDevice_Vulkan :public RenderDevice
	{
	public:

		RenderDevice_Vulkan(HWND mainscreen, bool fullscreen = false, bool debuglayer = false);
		~RenderDevice_Vulkan();


	private:

		VkInstance instance;
		VkDebugReportCallbackEXT callback;
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		QueueFamilyIndices queueIndices;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkPhysicalDeviceProperties physicalDeviceProperties;

		VkQueue copyQueue;
		VkCommandPool copyCommandPool;
		VkCommandBuffer copyCommandBuffer;
		VkFence copyFence;
		std::mutex copyQueueLock;

		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;

		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImage> swapChainImages;

		VkRenderPass defaultRenderpass;
		VkPipelineLayout defaultPiplineLayout_Graphics;
		VkPipelineLayout defaultPipelineLayout_Compute;
		VkDescriptorSetLayout defaultDescriptorSetLayouts[STAGE_COUNT];
		uint32_t descriptorCount;






	};
}


