#pragma once
#include "Render/RenderDevice.h"
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"


#include<optional>

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


	class RenderDevice_Vulkan :
		public RenderDevice
	{
	public:
		RenderDevice_Vulkan();
		~RenderDevice_Vulkan();
	};
}


