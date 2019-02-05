#include "RenderDevice.h"

namespace PRE {

	

	bool RenderDevice::CheckCapability(GRAPHICSDEVICE_CAPABILITY capability)
	{

		switch (capability)
		{
		case PRE::RenderDevice::GRAPHICSDEVICE_CAPABILITY_TESSELLATION:
			return TESSELLATION;
			break;
		case PRE::RenderDevice::GRAPHICSDEVICE_CAPABILITY_MULTITHREAD_RENDERING:
			return MULTITHREAD_RENDERING;
			break;
		case PRE::RenderDevice::GRAPHICSDEVICE_CAPABILITY_CONSERVATIVE_RASTERIZATION:
			return CONSERVATIVE_RASTERIZATION;
			break;
		case PRE::RenderDevice::GRAPHICSDEVICE_CAPABILITY_RASTERIZER_ORDERED_VIEWS:
			return RASTERIZER_ORDERED_VIEWS;
			break;
		case PRE::RenderDevice::GRAPHICSDEVICE_CAPABILITY_UNORDEREDACCESSTEXTURE_LOAD_FORMAT_EXT:
			return UNORDEREDACCESSTEXTURE_LOAD_EXT;
			break;
		default:
			break;
		}
		return false;
	}

	uint32_t RenderDevice::GetFormatStride(FORMAT value)
	{

		switch (value)
		{
		case FORMAT_R32G32B32A32_FLOAT:
			return 16;
			break;
		case FORMAT_R32G32_FLOAT:
			return 8;
			break;
		case FORMAT_R11G11B10_FLOAT:
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R32_FLOAT:
		case FORMAT_R32_UINT:
		case FORMAT_R8G8B8A8_UINT:
		case FORMAT_R8G8B8A8_SINT:
		case FORMAT_R8G8B8A8_UNORM:
		case FORMAT_R8G8B8A8_SNORM:
		case FORMAT_R10G10B10A2_UNORM:
			return 4;
			break;
		case FORMAT_R16_FLOAT:
			return 2;
			break;
		}


		return 16;
	}

	RenderDevice::~RenderDevice()
	{

	}
}