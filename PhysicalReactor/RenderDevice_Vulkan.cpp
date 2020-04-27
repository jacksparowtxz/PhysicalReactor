#include "stdafx.h"
#include "RenderDevice_Vulkan.h"
#include<vector>
#include<iostream>
#include<set>
#include<string>

namespace PRE
{
	constexpr VkFormat ConvertFormat(FORMAT value)
	{
		switch (value)
		{
		case FORMAT_UNKNOWN:
			return VK_FORMAT_UNDEFINED;
			break;
		case FORMAT_R32G32B32A32_FLOAT:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		case FORMAT_R32G32B32A32_UINT:
			return VK_FORMAT_R32G32B32A32_UINT;
			break;
		case FORMAT_R32G32B32A32_SINT:
			return VK_FORMAT_R32G32B32A32_SINT;
			break;
		case FORMAT_R32G32B32_FLOAT:
			return VK_FORMAT_R32G32B32_SFLOAT;
			break;
		case FORMAT_R32G32B32_UINT:
			return VK_FORMAT_R32G32B32_UINT;
			break;
		case FORMAT_R32G32B32_SINT:
			return VK_FORMAT_R32G32B32_SINT;
			break;
		case FORMAT_R16G16B16A16_FLOAT:
			return VK_FORMAT_R16G16B16A16_SFLOAT;
			break;
		case FORMAT_R16G16B16A16_UNORM:
			return VK_FORMAT_R16G16B16A16_UNORM;
			break;
		case FORMAT_R16G16B16A16_UINT:
			return VK_FORMAT_R16G16B16A16_UINT;
			break;
		case FORMAT_R16G16B16A16_SNORM:
			return VK_FORMAT_R16G16B16A16_SNORM;
			break;
		case FORMAT_R16G16B16A16_SINT:
			return VK_FORMAT_R16G16B16A16_SINT;
			break;
		case FORMAT_R32G32_FLOAT:
			return VK_FORMAT_R32G32_SFLOAT;
			break;
		case FORMAT_R32G32_UINT:
			return VK_FORMAT_R32G32_UINT;
			break;
		case FORMAT_R32G32_SINT:
			return VK_FORMAT_R32G32_SINT;
			break;
		case FORMAT_R32G8X24_TYPELESS:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;
			break;
		case FORMAT_D32_FLOAT_S8X24_UINT:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;
			break;
		case FORMAT_R10G10B10A2_UNORM:
			return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
			break;
		case FORMAT_R10G10B10A2_UINT:
			return VK_FORMAT_A2B10G10R10_UINT_PACK32;
			break;
		case FORMAT_R11G11B10_FLOAT:
			return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
			break;
		case FORMAT_R8G8B8A8_UNORM:
			return VK_FORMAT_R8G8B8A8_UNORM;
			break;
		case FORMAT_R8G8B8A8_UNORM_SRGB:
			return VK_FORMAT_R8G8B8A8_SRGB;
			break;
		case FORMAT_R8G8B8A8_UINT:
			return VK_FORMAT_R8G8B8A8_UINT;
			break;
		case FORMAT_R8G8B8A8_SNORM:
			return VK_FORMAT_R8G8B8A8_SNORM;
			break;
		case FORMAT_R8G8B8A8_SINT:
			return VK_FORMAT_R8G8B8A8_SINT;
			break;
		case FORMAT_R16G16_FLOAT:
			return VK_FORMAT_R16G16_SFLOAT;
			break;
		case FORMAT_R16G16_UNORM:
			return VK_FORMAT_R16G16_UNORM;
			break;
		case FORMAT_R16G16_UINT:
			return VK_FORMAT_R16G16_UINT;
			break;
		case FORMAT_R16G16_SNORM:
			return VK_FORMAT_R16G16_SNORM;
			break;
		case FORMAT_R16G16_SINT:
			return VK_FORMAT_R16G16_SINT;
			break;
		case FORMAT_R32_TYPELESS:
			return VK_FORMAT_D32_SFLOAT;
			break;
		case FORMAT_D32_FLOAT:
			return VK_FORMAT_D32_SFLOAT;
			break;
		case FORMAT_R32_FLOAT:
			return VK_FORMAT_R32_SFLOAT;
			break;
		case FORMAT_R32_UINT:
			return VK_FORMAT_R32_UINT;
			break;
		case FORMAT_R32_SINT:
			return VK_FORMAT_R32_SINT;
			break;
		case FORMAT_R24G8_TYPELESS:
			return VK_FORMAT_D24_UNORM_S8_UINT;
			break;
		case FORMAT_D24_UNORM_S8_UINT:
			return VK_FORMAT_D24_UNORM_S8_UINT;
			break;
		case FORMAT_R8G8_UNORM:
			return VK_FORMAT_R8G8_UNORM;
			break;
		case FORMAT_R8G8_UINT:
			return VK_FORMAT_R8G8_UINT;
			break;
		case FORMAT_R8G8_SNORM:
			return VK_FORMAT_R8G8_SNORM;
			break;
		case FORMAT_R8G8_SINT:
			return VK_FORMAT_R8G8_SINT;
			break;
		case FORMAT_R16_TYPELESS:
			return VK_FORMAT_D16_UNORM;
			break;
		case FORMAT_R16_FLOAT:
			return VK_FORMAT_R16_SFLOAT;
			break;
		case FORMAT_D16_UNORM:
			return VK_FORMAT_D16_UNORM;
			break;
		case FORMAT_R16_UNORM:
			return VK_FORMAT_R16_UNORM;
			break;
		case FORMAT_R16_UINT:
			return VK_FORMAT_R16_UINT;
			break;
		case FORMAT_R16_SNORM:
			return VK_FORMAT_R16_SNORM;
			break;
		case FORMAT_R16_SINT:
			return VK_FORMAT_R16_SINT;
			break;
		case FORMAT_R8_UNORM:
			return VK_FORMAT_R8_UNORM;
			break;
		case FORMAT_R8_UINT:
			return VK_FORMAT_R8_UINT;
			break;
		case FORMAT_R8_SNORM:
			return VK_FORMAT_R8_SNORM;
			break;
		case FORMAT_R8_SINT:
			return VK_FORMAT_R8_SINT;
			break;
		case FORMAT_BC1_UNORM:
			return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
			break;
		case FORMAT_BC1_UNORM_SRGB:
			return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
			break;
		case FORMAT_BC2_UNORM:
			return VK_FORMAT_BC2_UNORM_BLOCK;
			break;
		case FORMAT_BC2_UNORM_SRGB:
			return VK_FORMAT_BC2_SRGB_BLOCK;
			break;
		case FORMAT_BC3_UNORM:
			return VK_FORMAT_BC3_UNORM_BLOCK;
			break;
		case FORMAT_BC3_UNORM_SRGB:
			return VK_FORMAT_BC3_SRGB_BLOCK;
			break;
		case FORMAT_BC4_UNORM:
			return VK_FORMAT_BC4_UNORM_BLOCK;
			break;
		case FORMAT_BC4_SNORM:
			return VK_FORMAT_BC4_SNORM_BLOCK;
			break;
		case FORMAT_BC5_UNORM:
			return VK_FORMAT_BC5_UNORM_BLOCK;
			break;
		case FORMAT_BC5_SNORM:
			return VK_FORMAT_BC5_SNORM_BLOCK;
			break;
		case FORMAT_B8G8R8A8_UNORM:
			return VK_FORMAT_B8G8R8A8_UNORM;
			break;
		case FORMAT_B8G8R8A8_UNORM_SRGB:
			return VK_FORMAT_B8G8R8A8_SRGB;
			break;
		case FORMAT_BC6H_UF16:
			return VK_FORMAT_BC6H_UFLOAT_BLOCK;
			break;
		case FORMAT_BC6H_SF16:
			return VK_FORMAT_BC6H_SFLOAT_BLOCK;
			break;
		case FORMAT_BC7_UNORM:
			return VK_FORMAT_BC7_UNORM_BLOCK;
			break;
		case FORMAT_BC7_UNORM_SRGB:
			return VK_FORMAT_BC7_SRGB_BLOCK;
			break;
		}
		return VK_FORMAT_UNDEFINED;
	      
	
	
	
	}

	constexpr VkCompareOp ConvertComparisonFunc(COMPARSION_FUNC value)
	{
		switch (value)
		{
		case PRE::COMPARISON_NEVER:
			return VK_COMPARE_OP_NEVER;
			break;
		case COMPARISON_LESS:
			return VK_COMPARE_OP_LESS;
			break;
		case COMPARISON_EQUAL:
			return VK_COMPARE_OP_EQUAL;
			break;
		case COMPARISON_LESS_EQUAL:
			return VK_COMPARE_OP_LESS_OR_EQUAL;
			break;
		case COMPARISON_GREATER:
			return VK_COMPARE_OP_GREATER;
			break;
		case COMPARISON_NOT_EQUAL:
			return VK_COMPARE_OP_NOT_EQUAL;
			break;
		case COMPARISON_GREATER_EQUAL:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;
			break;
		case COMPARISON_ALWAYS:
			return VK_COMPARE_OP_ALWAYS;
			break;
		default:
			break;
		}
	
		return VK_COMPARE_OP_NEVER;
	
	}

	constexpr VkBlendFactor ConvertBlend(BLEND value)
	{
		switch (value)
		{
		case BLEND_ZERO:
			return VK_BLEND_FACTOR_ZERO;
			break;
		case BLEND_ONE:
			return VK_BLEND_FACTOR_ONE;
			break;
		case BLEND_SRC_COLOR:
			return VK_BLEND_FACTOR_SRC_COLOR;
			break;
		case BLEND_INV_SRC_COLOR:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			break;
		case BLEND_SRC_ALPHA:
			return VK_BLEND_FACTOR_SRC_ALPHA;
			break;
		case BLEND_INV_SRC_ALPHA:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
		case BLEND_DEST_ALPHA:
			return VK_BLEND_FACTOR_DST_ALPHA;
			break;
		case BLEND_INV_DEST_ALPHA:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			break;
		case BLEND_DEST_COLOR:
			return VK_BLEND_FACTOR_DST_COLOR;
			break;
		case BLEND_INV_DEST_COLOR:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			break;
		case BLEND_SRC_ALPHA_SAT:
			return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			break;
		case BLEND_BLEND_FACTOR:
			return VK_BLEND_FACTOR_CONSTANT_COLOR;
			break;
		case BLEND_INV_BLEND_COLOR:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			break;
		case BLEND_SRC1_COLOR:
			return VK_BLEND_FACTOR_SRC1_COLOR;
			break;
		case BLEND_INV_SRC1_COLOR:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			break;
		case BLEND_SRC1_ALPHA:
			return VK_BLEND_FACTOR_SRC1_ALPHA;
			break;
		case BLEND_INV_SRC1_ALPHA:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			break;
		default:
			break;
		}
		return VK_BLEND_FACTOR_ZERO;
	
	}

	constexpr VkBlendOp ConvertBlendOp(BLEND_OP value)
	{
		switch (value)
		{
		case BLEND_OP_ADD:
			return VK_BLEND_OP_ADD;
			break;
		case BLEND_OP_SUBTRACT:
			return VK_BLEND_OP_SUBTRACT;
			break;
		case BLEND_OP_REV_SUBTRACT:
			return VK_BLEND_OP_REVERSE_SUBTRACT;
			break;
		case BLEND_OP_MIN:
			return VK_BLEND_OP_MIN;
			break;
		case BLEND_OP_MAX:
			return VK_BLEND_OP_MAX;
			break;
		default:
			break;
		}
		return VK_BLEND_OP_ADD;
	
	}

	constexpr VkSamplerAddressMode _ConvertTextureAddressMode(TEXTURE_ADDRESS_MODE value)
	{
		switch (value)
		{
		case TEXTURE_ADDRESS_WRAP:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			break;
		case TEXTURE_ADDRESS_MIRROR:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			break;
		case TEXTURE_ADDRESS_CLAMP:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			break;
		case TEXTURE_ADDRESS_BORDER:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			break;
		default:
			break;
		}
		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	}

	constexpr VkStencilOp _ConvertStencilOp(STENCIL_OP value)
	{
		switch (value)
		{
		case STENCIL_OP_KEEP:
			return VK_STENCIL_OP_KEEP;
			break;
		case STENCIL_OP_ZERO:
			return VK_STENCIL_OP_ZERO;
			break;
		case STENCIL_OP_REPLACE:
			return VK_STENCIL_OP_REPLACE;
			break;
		case STENCIL_OP_INCR_SAT:
			return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			break;
		case STENCIL_OP_DECR_SAT:
			return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			break;
		case STENCIL_OP_INVERT:
			return VK_STENCIL_OP_INVERT;
			break;
		case STENCIL_OP_INCR:
			return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			break;
		case STENCIL_OP_DECR:
			return VK_STENCIL_OP_DECREMENT_AND_WRAP;
			break;
		default:
			break;
		}
		return VK_STENCIL_OP_KEEP;
	}

	constexpr VkImageLayout _ConvertImageLayout(IMAGE_LAYOUT value)
	{
		switch (value)
		{
		case IMAGE_LAYOUT_GENERAL:
			return VK_IMAGE_LAYOUT_GENERAL;
		case IMAGE_LAYOUT_RENDERTARGET:
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		case IMAGE_LAYOUT_DEPTHSTENCIL:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		case IMAGE_LAYOUT_DEPTHSTENCIL_READONLY:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		case IMAGE_LAYOUT_SHADER_RESOURCE:
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		case IMAGE_LAYOUT_UNORDERED_ACCESS:
			return VK_IMAGE_LAYOUT_GENERAL;
		case IMAGE_LAYOUT_COPY_SRC:
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		case IMAGE_LAYOUT_COPY_DST:
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		}
		return VK_IMAGE_LAYOUT_UNDEFINED;
	}


	inline VkAccessFlags ParseImageLayout(IMAGE_LAYOUT value)
	{
		VkAccessFlags flags = 0;

		switch (value)
		{
		case IMAGE_LAYOUT_GENERAL:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_SHADER_WRITE_BIT;
			flags |= VK_ACCESS_TRANSFER_READ_BIT;
			flags |= VK_ACCESS_TRANSFER_WRITE_BIT;
			flags |= VK_ACCESS_MEMORY_READ_BIT;
			flags |= VK_ACCESS_MEMORY_WRITE_BIT;
			break;
		case IMAGE_LAYOUT_RENDERTARGET:
			flags |= VK_ACCESS_SHADER_WRITE_BIT;
			break;
		case IMAGE_LAYOUT_DEPTHSTENCIL:
			flags |= VK_ACCESS_SHADER_WRITE_BIT;
			break;
		case IMAGE_LAYOUT_DEPTHSTENCIL_READONLY:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			break;
		case IMAGE_LAYOUT_SHADER_RESOURCE:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			break;
		case IMAGE_LAYOUT_UNORDERED_ACCESS:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_SHADER_WRITE_BIT;
			break;
		case IMAGE_LAYOUT_COPY_SRC:
			flags |= VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case IMAGE_LAYOUT_COPY_DST:
			flags |= VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		}

		return flags;
	}

	inline VkAccessFlags _ParseBufferState(BUFFER_STATE value)
	{
		VkAccessFlags flags = 0;

		switch (value)
		{
		case BUFFER_STATE_GENERAL:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_SHADER_WRITE_BIT;
			flags |= VK_ACCESS_TRANSFER_READ_BIT;
			flags |= VK_ACCESS_TRANSFER_WRITE_BIT;
			flags |= VK_ACCESS_HOST_READ_BIT;
			flags |= VK_ACCESS_HOST_WRITE_BIT;
			flags |= VK_ACCESS_MEMORY_READ_BIT;
			flags |= VK_ACCESS_MEMORY_WRITE_BIT;
			flags |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
			flags |= VK_ACCESS_INDEX_READ_BIT;
			flags |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
			flags |= VK_ACCESS_UNIFORM_READ_BIT;
			break;
		case BUFFER_STATE_VERTEX_BUFFER:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
			break;
		case BUFFER_STATE_INDEX_BUFFER:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_INDEX_READ_BIT;
			break;
		case BUFFER_STATE_CONSTANT_BUFFER:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_UNIFORM_READ_BIT;
			break;
		case BUFFER_STATE_INDIRECT_ARGUMENT:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
			break;
		case BUFFER_STATE_SHADER_RESOURCE:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_UNIFORM_READ_BIT;
			break;
		case BUFFER_STATE_UNORDERED_ACCESS:
			flags |= VK_ACCESS_SHADER_READ_BIT;
			flags |= VK_ACCESS_SHADER_WRITE_BIT;
			break;
		case BUFFER_STATE_COPY_SRC:
			flags |= VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case BUFFER_STATE_COPY_DST:
			flags |= VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		default:
			break;
		}

		return flags;
	}

	PFN_vkSetDebugUtilsObjectNameEXT setDebugUtilsObjectNameEXT;
	PFN_vkCmdBeginDebugUtilsLabelEXT cmdBeginDebugUtilsLabelEXT;
	PFN_vkCmdEndDebugUtilsLabelEXT cmdEndDebugUtilsLabelEXT;
	PFN_vkCmdInsertDebugUtilsLabelEXT cmdInsertDebugUtilsLabelEXT;

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
	};


	bool checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount,availableLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProerties : availableLayers)
			{
				if (strcmp(layerName, layerProerties.layerName) == 0)
				{
					break;
				}

				if (!layerFound)
				{
					return false;
				}
			}
		}
	
		return true;
	
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flag,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* pUserData)
	{
		std::stringstream ss("");
		ss << "[VULKAN validation layer]:" << msg << std::endl;

		std::cerr << ss.str();
		OutputDebugStringA(ss.str().c_str());
	 
		return VK_FALSE;
	
	}


	VkResult CreateDebugReportCallbackEXT(VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugReportCallbackEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}


	void DestroyDebugReportCallbackEXT(VkInstance instance,
		VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDetroyDebugReportCallEXT");
		if (func != nullptr)
		{
			func(instance, callback, pAllocator);
		}
	}

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,VkSurfaceKHR surface)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}




	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		QueueFamilyIndices indices = findQueueFamilies(device, surface);

		return indices.graphicsFamily.has_value();
	}

	const std::vector<const char*> deviceExtensions = {
	  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	  VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME,
	};
	
	bool checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}
	    
		return requiredExtensions.empty();
	}




	RenderDevice_Vulkan::RenderDevice_Vulkan()
	{


		 







	}


	RenderDevice_Vulkan::~RenderDevice_Vulkan()
	{
	}

}
