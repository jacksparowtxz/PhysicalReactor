#include "RenderDevice_DX11.h"
#include "ResourceMapping.h"
#include "MISC/MiscHelper.h"

#include <sstream>

#include <functional>

#include <d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"Dxgi.lib")
#pragma comment(lib,"dxguid.lib")


using namespace std;


namespace PRE
{


	inline UINT ParseBindFlags(UINT value)
	{
		UINT flag = 0;

		if (value & BIND_VERTEX_BUFFER)
			flag |= D3D11_BIND_VERTEX_BUFFER;
		if (value & BIND_INDEX_BUFFER)
			flag |= D3D11_BIND_INDEX_BUFFER;
		if (value & BIND_CONSTANT_BUFFER)
			flag |= D3D11_BIND_CONSTANT_BUFFER;
		if (value & BIND_SHADER_RESOURCE)
			flag |= D3D11_BIND_SHADER_RESOURCE;
		if (value & BIND_STREAM_OUTPUT)
			flag |= D3D11_BIND_STREAM_OUTPUT;
		if (value & BIND_RENDER_TARGET)
			flag |= D3D11_BIND_RENDER_TARGET;
		if (value & BIND_DEPTH_STENCIL)
			flag |= D3D11_BIND_DEPTH_STENCIL;
		if (value & BIND_UNORDERED_ACCESS)
			flag |= D3D11_BIND_UNORDERED_ACCESS;
		return flag;
	}

	inline UINT ParseCPUAccessFlags(UINT value)
	{
		UINT flag = 0;
		if (value & CPU_ACCESS_WRITE)
		{
			flag |= D3D11_CPU_ACCESS_WRITE;
		}
		if (value & CPU_ACCESS_READ)
		{
			flag |= D3D11_CPU_ACCESS_READ;
		}

		return flag;
	}

	inline UINT ParseResourceMiscFlags(UINT value)
	{
		UINT flag = 0;

		if (value & RESOURCE_MISC_SHARED)
		{
			flag |= D3D11_RESOURCE_MISC_SHARED;
		}
		if (value & RESOURCE_MISC_TEXTURECUBE)
		{
			flag |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		}
		if (value & RESOURCE_MISC_DRAWINDIRECT_ARGS)
		{
			flag |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		}
		if (value & RESOUCRE_MISC_BUFFER_ALLOW_RAW_VIEWS)
		{
			flag |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}
		if (value & RESOUCRE_MISC_BUFFER_STRUCTURED)
		{
			flag |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		}
		if (value & RESOUCR_MISC_TILED)
		{
			flag |= D3D11_RESOURCE_MISC_TILED;
		}

		return flag;
	}

	inline UINT ParseColorWriteMask(UINT value)
	{
		UINT flag = 0;

		if (value == D3D11_COLOR_WRITE_ENABLE_ALL)
		{
			return D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		else
		{
			if (value & COLOR_WRITE_ENABLE_RED)
			{
				flag |= D3D11_COLOR_WRITE_ENABLE_RED;
			}
			if (value & COLOR_WRITE_ENABLE_GREEN)
			{
				flag |= D3D11_COLOR_WRITE_ENABLE_GREEN;
			}
			if (value & COLOR_WRITE_ENABLE_BLUE)
			{
				flag |= D3D11_COLOR_WRITE_ENABLE_BLUE;
			}
			if (value &COLOR_WRITE_ENABLE_ALPHA)
			{
				flag |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
			}
			return flag;
		}
	}

	inline D3D11_FILTER ConvertFilter(FILTER value)
	{
		switch (value)
		{
		case PRE::FILTER_MIN_MAG_MIP_POINT:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MIN_MAG_POINT_MIP_LINEAR:
			return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT:
			return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MIN_POINT_MAG_MIP_LINEAR:
			return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MIN_LINEAR_MAG_MIP_POINT:
			return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MIN_MAG_LINEAR_MIP_POINT:
			return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MIN_MAG_MIP_LINEAR:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_ANISOTROPIC:
			return D3D11_FILTER_ANISOTROPIC;
			break;
		case PRE::FILTER_COMPARISON_MIN_MAG_MIP_POINT:
			return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
			break;
		case PRE::FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
			return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
			return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
			return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_COMPARSION_LINEAR_MAG_MIP_POINT:
			return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case PRE::FILTER_COMPARSION_LINEAR_MAG_POINT_MIP_LINEAR:
			return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_COMPARSION_MIN_MAG_LINEAR_MIP_POINT:
			return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_COMPARSION_MIN_MAG_MIP_LINEAR:
			return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_COMPARSION_ANISOTROPIC:
			return D3D11_FILTER_COMPARISON_ANISOTROPIC;
			break;
		case PRE::FILTER_MINIMUM_MIN_MAG_MIP_POINT:
			return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MINMUM_MIN_MAG_POINT_MIP_LINEAR:
			return D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MINMUM_MIN_POINT_MAG_POINT_LINEAR_MIP_POINT:
			return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MINMUM_MIN_POINT_MAG_MIP_LINEAR:
			return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MINMUM_MIN_LINEAR_MAG_MIP_POINT:
			return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MINMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MINMUM_MIN_MAG_LINEAR_MIP_POINT:
			return D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MINMUM_MIN_MAG_MIN_LINEAR:
			return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MINMUM_ANSOTROPIC:
			return D3D11_FILTER_MINIMUM_ANISOTROPIC;
			break;
		case PRE::FILTER_MAXIMUM_MIN_MAG_MIP_POINT:
			return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MAXIMUM_MIN_MAG_POINT_MIP_POINT:
			return D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT:
			return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR:
			return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT:
			return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT:
			return D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR:
			return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MAXIMUM_ANISOTROPIC:
			return D3D11_FILTER_MAXIMUM_ANISOTROPIC;
			break;
		default:
			break;
		}
		return D3D11_FILTER_MAXIMUM_ANISOTROPIC;
	}

	inline D3D11_TEXTURE_ADDRESS_MODE ConvertTextureAddressMode(TEXTURE_ADDRESS_MODE value)
	{
		switch (value)
		{
		case PRE::TEXTURE_ADDRESS_WRAP:
			return D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		case PRE::TEXTURE_ADDRESS_MIRROR:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
			break;
		case PRE::TEXTURE_ADDRESS_CLAMP:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
			break;
		case PRE::TEXTURE_ADDRESS_BORDER:
			return D3D11_TEXTURE_ADDRESS_BORDER;
			break;
		case PRE::TEXTURE_ADDRESS_MIRROR_ONCE:
			return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
			break;
		default:
			break;
		}
		return D3D11_TEXTURE_ADDRESS_WRAP;
	}

	inline D3D11_COMPARISON_FUNC ConvertComparisonFunc(COMPARSION_FUNC value)
	{
		switch (value)
		{
		case PRE::COMPARSION_NEVER:
			return D3D11_COMPARISON_NEVER;
			break;
		case PRE::COMPARSION_LESS:
			return D3D11_COMPARISON_LESS;
			break;
		case PRE::COMPARSION_EQUAL:
			return D3D11_COMPARISON_EQUAL;
			break;
		case PRE::COMPARSION_LESS_EQUAL:
			return D3D11_COMPARISON_LESS_EQUAL;
			break;
		case PRE::COMPARSION_GREATER:
			return D3D11_COMPARISON_GREATER;
			break;
		case PRE::COMPARSION_NOT_EQUAL:
			return D3D11_COMPARISON_NOT_EQUAL;
			break;
		case PRE::COMPARSION_GREATER_EQUAL:
			return D3D11_COMPARISON_GREATER_EQUAL;
			break;
		case PRE::COMPARSION_ALWAYS:
			return D3D11_COMPARISON_ALWAYS;
			break;
		default:
			break;
		}
		return D3D11_COMPARISON_NEVER;
	}

	inline D3D11_FILL_MODE ConvertFillMode(FILL_MODE value)
	{
		switch (value)
		{
		case PRE::FILL_WIREFRAME:
			return D3D11_FILL_WIREFRAME;
			break;
		case PRE::FILL_SOLID:
			return D3D11_FILL_SOLID;
			break;
		default:
			break;
		}
		return D3D11_FILL_WIREFRAME;
	}


	inline D3D11_CULL_MODE ConvertCullMode(CULL_MODE value)
	{
		switch (value)
		{
		case PRE::CULL_NONE:
			return D3D11_CULL_NONE;
			break;
		case PRE::CULL_FRONT:
			return D3D11_CULL_FRONT;
			break;
		case PRE::CULL_BACK:
			return D3D11_CULL_BACK;
			break;
		default:
			break;
		}
		return D3D11_CULL_NONE;
	}

	inline D3D11_DEPTH_WRITE_MASK ConvertDepthWriteMask(DEPTH_WRITE_MASK value)
	{
		switch (value)
		{
		case PRE::DEPTH_WRITE_MASK_ZERO:
			return D3D11_DEPTH_WRITE_MASK_ZERO;
			break;
		case PRE::DEPTH_WRITE_MASK_ALL:
			return D3D11_DEPTH_WRITE_MASK_ALL;
			break;
		default:
			break;
		}
		return D3D11_DEPTH_WRITE_MASK_ZERO;
	}

	inline D3D11_STENCIL_OP ConvertStencilOp(STENCIL_OP value)
	{
		switch (value)
		{
		case PRE::STENCIL_OP_KEEP:
			return D3D11_STENCIL_OP_KEEP;
			break;
		case PRE::STENCIL_OP_ZERO:
			return D3D11_STENCIL_OP_ZERO;
			break;
		case PRE::STENCIL_OP_REPLACE:
			return D3D11_STENCIL_OP_REPLACE;
			break;
		case PRE::STENCIL_OP_INCR_SAT:
			return D3D11_STENCIL_OP_INCR_SAT;
			break;
		case PRE::STENCIL_OP_DECR_SAT:
			return D3D11_STENCIL_OP_DECR_SAT;
			break;
		case PRE::STENCIL_OP_INVERT:
			return D3D11_STENCIL_OP_INVERT;
			break;
		case PRE::STENCIL_OP_INCR:
			return D3D11_STENCIL_OP_INCR;
			break;
		case PRE::STENCIL_OP_DECR:
			return D3D11_STENCIL_OP_DECR;
			break;
		default:
			break;
		}

		return D3D11_STENCIL_OP_KEEP;
	}

	inline D3D11_BLEND ConvertBlend(BLEND value)
	{
		switch (value)
		{
		case PRE::BLEND_ZERO:
			return D3D11_BLEND_ZERO;
			break;
		case PRE::BLEND_ONE:
			return D3D11_BLEND_ONE;
			break;
		case PRE::BLEND_SRC_COLOR:
			return D3D11_BLEND_SRC_COLOR;
			break;
		case PRE::BLEND_INV_SRC_COLOR:
			return D3D11_BLEND_INV_SRC_COLOR;
			break;
		case PRE::BLEND_SRC_ALPHA:
			return D3D11_BLEND_SRC_ALPHA;
			break;
		case PRE::BLEND_INV_SRC_ALPHA:
			return D3D11_BLEND_INV_SRC_ALPHA;
			break;
		case PRE::BLEND_DEST_ALPHA:
			return D3D11_BLEND_DEST_ALPHA;
			break;
		case PRE::BLNED_INV_DEST_ALPHA:
			return D3D11_BLEND_INV_DEST_ALPHA;
			break;
		case PRE::BLEND_DEST_COLOR:
			return D3D11_BLEND_DEST_COLOR;
			break;
		case PRE::BLEND_INV_DEST_COLOR:
			return D3D11_BLEND_INV_DEST_COLOR;
			break;
		case PRE::BLNED_SRC_ALPHA_SAT:
			return D3D11_BLEND_SRC_ALPHA_SAT;
			break;
		case PRE::BLEND_BLEND_FACTOR:
			return D3D11_BLEND_BLEND_FACTOR;
			break;
		case PRE::BLNED_INV_BLNED_COLOR:
			return D3D11_BLEND_INV_BLEND_FACTOR;
			break;
		case PRE::BLNED_SRC1_COLOR:
			return D3D11_BLEND_SRC1_COLOR;
			break;
		case PRE::BLEND_INV_SRC1_COLOR:
			return D3D11_BLEND_INV_SRC1_COLOR;
			break;
		case PRE::BLNED_SRC1_ALPHA:
			return D3D11_BLEND_SRC1_ALPHA;
			break;
		case PRE::BLNED_INV_SRC1_ALPHA:
			return D3D11_BLEND_INV_SRC1_ALPHA;
			break;
		default:
			break;
		}
		return D3D11_BLEND_ZERO;
	}

	inline D3D11_BLEND_OP ConvertBlendOp(BLEND_OP value)
	{
		switch (value)
		{
		case PRE::BLEND_OP_ADD:
			return D3D11_BLEND_OP_ADD;
			break;
		case PRE::BLEND_OP_SUBTRACT:
			return D3D11_BLEND_OP_SUBTRACT;
			break;
		case PRE::BLEND_OP_REV_SUBTRACT:
			return D3D11_BLEND_OP_REV_SUBTRACT;
			break;
		case PRE::BLEND_OP_MIN:
			return D3D11_BLEND_OP_MIN;
			break;
		case PRE::BLEND_OP_MAX:
			return D3D11_BLEND_OP_MAX;
			break;
		default:
			break;
		}
		return D3D11_BLEND_OP_ADD;
	}

	inline D3D11_USAGE ConvertUsage(USAGE value)
	{
		switch (value)
		{
		case PRE::USAGE_DEFAULT:
			return D3D11_USAGE_DEFAULT;
			break;
		case PRE::USAGE_IMMUTABLE:
			return D3D11_USAGE_IMMUTABLE;
			break;
		case PRE::USAGE_DYNAMIC:
			return D3D11_USAGE_DYNAMIC;
			break;
		case PRE::USAGE_STAGING:
			return D3D11_USAGE_STAGING;
			break;
		default:
			break;
		}
		return D3D11_USAGE_DEFAULT;
	}

	inline D3D11_INPUT_CLASSIFICATION ConvertInputClassification(INPUT_CLASSIFICATION value)
	{
		switch (value)
		{
		case PRE::INPUT_PER_VERTEX_DATA:
			return D3D11_INPUT_PER_VERTEX_DATA;
			break;
		case PRE::INPUT_PER_INSTANCE_DATA:
			return D3D11_INPUT_PER_INSTANCE_DATA;
			break;
		default:
			break;
		}
		return D3D11_INPUT_PER_VERTEX_DATA;
	}

	inline DXGI_FORMAT ConvertFormat(FORMAT value)
	{
		switch (value)
		{
		case PRE::FORMAT_UNKNOWN:
			return DXGI_FORMAT_UNKNOWN;
			break;
		case PRE::FORMAT_R32G32B32A32_TYPELESS:
			return DXGI_FORMAT_R32G32B32A32_TYPELESS;
			break;
		case PRE::FORMAT_R32G32B32A32_FLOAT:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case PRE::FORMAT_R32G32B32A32_UINT:
			return DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		case PRE::FORMAT_R32G32B32A32_SINT:
			return DXGI_FORMAT_R32G32B32A32_SINT;
			break;
		case PRE::FORMAT_R32G32B32_TYPELESS:
			return DXGI_FORMAT_R32G32B32_TYPELESS;
			break;
		case PRE::FORMAT_R32G32B32_FLOAT:
			return DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case PRE::FORMAT_R32G32B32_UINT:
			return DXGI_FORMAT_R32G32B32_UINT;
			break;
		case PRE::FORMAT_R32G32B32_SINT:
			return DXGI_FORMAT_R32G32B32_SINT;
			break;
		case PRE::FORMAT_R16G16B16A16_TYPELESS:
			return DXGI_FORMAT_R16G16B16A16_TYPELESS;
			break;
		case PRE::FORMAT_R16G16B16A16_FLOAT:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;
		case PRE::FORMAT_R16G16B16A16_UNORM:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
			break;
		case PRE::FORMAT_R16G16B16A16_UINT:
			return DXGI_FORMAT_R16G16B16A16_UINT;
			break;
		case PRE::FORMAT_R16G16B16A16_SNORM:
			return DXGI_FORMAT_R16G16B16A16_SNORM;
			break;
		case PRE::FORMAT_R16G16B16A16_SINT:
			return DXGI_FORMAT_R16G16B16A16_SINT;
			break;
		case PRE::FORMAT_R32G32_TYPELESS:
			return DXGI_FORMAT_R32G32_TYPELESS;
			break;
		case PRE::FORMAT_R32G32_FLOAT:
			return DXGI_FORMAT_R32G32_FLOAT;
			break;
		case PRE::FORMAT_R32G32_UINT:
			return DXGI_FORMAT_R32G32_UINT;
			break;
		case PRE::FORMAT_R32G32_SINT:
			return DXGI_FORMAT_R32G32_SINT;
			break;
		case PRE::FORMAT_R32G8X24_TYPELESS:
			return DXGI_FORMAT_R32G8X24_TYPELESS;
			break;
		case PRE::FORMAT_D32_FLOAT_S8X24_UINT:
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			break;
		case PRE::FORMAT_R32_FLOAT_X8X24_TYPELESS:
			return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			break;
		case PRE::FORMAT_X32_TYPELESS_G8X24_UINT:
			return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
			break;
		case PRE::FORMAT_R10G10B10A2_TYPELESS:
			return DXGI_FORMAT_R10G10B10A2_TYPELESS;
			break;
		case PRE::FORMAT_R10G10B10A2_UNORM:
			return DXGI_FORMAT_R10G10B10A2_UNORM;
			break;
		case PRE::FORMAT_R10G10B10A2_UINT:
			return DXGI_FORMAT_R10G10B10A2_UINT;
			break;
		case PRE::FORMAT_R11G11B10_FLOAT:
			return DXGI_FORMAT_R11G11B10_FLOAT;
			break;
		case PRE::FORMAT_R8G8B8A8_TYPELESS:
			return DXGI_FORMAT_R8G8B8A8_TYPELESS;
			break;
		case PRE::FORMAT_R8G8B8A8_UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case PRE::FORMAT_R8G8B8A8_UNORM_SRGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			break;
		case PRE::FORMAT_R8G8B8A8_UINT:
			return DXGI_FORMAT_R8G8B8A8_UINT;
			break;
		case PRE::FORMAT_R8G8B8A8_SNORM:
			return DXGI_FORMAT_R8G8B8A8_SNORM;
			break;
		case PRE::FORMAT_R8G8B8A8_SINT:
			return DXGI_FORMAT_R8G8B8A8_SINT;
			break;
		case PRE::FORMAT_R16G16_TYPELESS:
			return DXGI_FORMAT_R16G16_TYPELESS;
			break;
		case PRE::FORMAT_R16G16_FLOAT:
			return DXGI_FORMAT_R16G16_FLOAT;
			break;
		case PRE::FORMAT_R16G16_UNORM:
			return DXGI_FORMAT_R16G16_UNORM;
			break;
		case PRE::FORMAT_R16G16_UINT:
			return DXGI_FORMAT_R16G16_UINT;
			break;
		case PRE::FORMAT_R16G16_SNORM:
			return DXGI_FORMAT_R16G16_SNORM;
			break;
		case PRE::FORMAT_R16G16_SINT:
			return DXGI_FORMAT_R16G16_SINT;
			break;
		case PRE::FORMAT_R32_TYPELESS:
			return DXGI_FORMAT_R32_TYPELESS;
			break;
		case PRE::FORMAT_D32_FLOAT:
			return DXGI_FORMAT_D32_FLOAT;
			break;
		case PRE::FORMAT_R32_FLOAT:
			return DXGI_FORMAT_R32_FLOAT;
			break;
		case PRE::FORMAT_R32_UINT:
			return DXGI_FORMAT_R32_UINT;
			break;
		case PRE::FORMAT_R32_SINT:
			return DXGI_FORMAT_R32_SINT;
			break;
		case PRE::FORMAT_R24G8_TYPELESS:
			return DXGI_FORMAT_R24G8_TYPELESS;
			break;
		case PRE::FORMAT_D24_UNORM_S8_UINT:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		case PRE::FORMAT_R24_UNORM_X8_TYPELESS:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case PRE::FORMAT_X24_TYPELESS_G8_UINT:
			return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
			break;
		case PRE::FORMAT_R8G8_TYPELESS:
			return DXGI_FORMAT_R8G8_TYPELESS;
			break;
		case PRE::FORMAT_R8G8_UNORM:
			return DXGI_FORMAT_R8G8_UNORM;
			break;
		case PRE::FORMAT_R8G8_UINT:
			return DXGI_FORMAT_R8G8_UINT;
			break;
		case PRE::FORMAT_R8G8_SNORM:
			return DXGI_FORMAT_R8G8_SNORM;
			break;
		case PRE::FORMAT_R8G8_SINT:
			return DXGI_FORMAT_R8G8_SINT;
			break;
		case PRE::FORMAT_R16_TYPELESS:
			return DXGI_FORMAT_R16_TYPELESS;
			break;
		case PRE::FORMAT_R16_FLOAT:
			return DXGI_FORMAT_R16_FLOAT;
			break;
		case PRE::FORMAT_D16_UNORM:
			return DXGI_FORMAT_D16_UNORM;
			break;
		case PRE::FORMAT_R16_UNORM:
			return DXGI_FORMAT_R16_UNORM;
			break;
		case PRE::FORMAT_R16_UINT:
			return DXGI_FORMAT_R16_UINT;
			break;
		case PRE::FORMAT_R16_SNORM:
			return DXGI_FORMAT_R16_SNORM;
			break;
		case PRE::FORMAT_R16_SINT:
			return DXGI_FORMAT_R16_SINT;
			break;
		case PRE::FORMAT_R8_TYPELESS:
			return DXGI_FORMAT_R8_TYPELESS;
			break;
		case PRE::FORMAT_R8_UNORM:
			return DXGI_FORMAT_R8_UNORM;
			break;
		case PRE::FORMAT_R8_UINT:
			return DXGI_FORMAT_R8_UINT;
			break;
		case PRE::FORMAT_R8_SNORM:
			return DXGI_FORMAT_R8_SNORM;
			break;
		case PRE::FORMAT_R8_SINT:
			return DXGI_FORMAT_R8_SINT;
			break;
		case PRE::FORMAT_A8_UNORM:
			return DXGI_FORMAT_A8_UNORM;
			break;
		case PRE::FORMAT_R1_UNORM:
			return DXGI_FORMAT_R1_UNORM;
			break;
		case PRE::FORMAT_R9G9B9E5_SHAREDEXP:
			return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
			break;
		case PRE::FORMAT_R8G8_B8G8_UNORM:
			return DXGI_FORMAT_R8G8_B8G8_UNORM;
			break;
		case PRE::FORMAT_G8R8_G8B8_UNORM:
			return DXGI_FORMAT_G8R8_G8B8_UNORM;
			break;
		case PRE::FORMAT_BC1_TYPELESS:
			return DXGI_FORMAT_BC1_TYPELESS;
			break;
		case PRE::FORMAT_BC1_UNORM:
			return DXGI_FORMAT_BC1_UNORM;
			break;
		case PRE::FORMAT_BC1_UNORM_SRGB:
			return DXGI_FORMAT_BC1_UNORM_SRGB;
			break;
		case PRE::FORMAT_BC2_TYPELESS:
			return DXGI_FORMAT_BC2_TYPELESS;
			break;
		case PRE::FORMAT_BC2_UNORM:
			return DXGI_FORMAT_BC2_UNORM;
			break;
		case PRE::FORMAT_BC2_UNORM_SRGB:
			return DXGI_FORMAT_BC2_UNORM_SRGB;
			break;
		case PRE::FORMAT_BC3_TYPELESS:
			return DXGI_FORMAT_BC3_TYPELESS;
			break;
		case PRE::FORMAT_BC3_UNORM:
			return DXGI_FORMAT_BC3_UNORM;
			break;
		case PRE::FORMAT_BC3_UNORM_SRGB:
			return DXGI_FORMAT_BC3_UNORM_SRGB;
			break;
		case PRE::FORMAT_BC4_TYPELESS:
			return DXGI_FORMAT_BC4_TYPELESS;
			break;
		case PRE::FORMAT_BC4_UNORM:
			return DXGI_FORMAT_BC4_UNORM;
			break;
		case PRE::FORMAT_BC4_SNORM:
			return DXGI_FORMAT_BC4_SNORM;
			break;
		case PRE::FORMAT_BC5_TYPELESS:
			return DXGI_FORMAT_BC5_TYPELESS;
			break;
		case PRE::FORMAT_BC5_UNORM:
			return DXGI_FORMAT_BC5_UNORM;
			break;
		case PRE::FORMAT_BC5_SNORM:
			return DXGI_FORMAT_BC5_SNORM;
			break;
		case PRE::FORMAT_B5G6R5_UNORM:
			return DXGI_FORMAT_B5G6R5_UNORM;
			break;
		case PRE::FORMAT_B5G5R5A1_UNORM:
			return DXGI_FORMAT_B5G5R5A1_UNORM;
			break;
		case PRE::FORMAT_B8G8R8A8_UNORM:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
			break;
		case PRE::FORMAT_B8G8R8X8_UNORM:
			return DXGI_FORMAT_B8G8R8X8_UNORM;
			break;
		case PRE::FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
			break;
		case PRE::FORMAT_B8G8R8A8_TYPELESS:
			return DXGI_FORMAT_B8G8R8A8_TYPELESS;
			break;
		case PRE::FORMAT_B8G8R8A8_UNORM_SRGB:
			return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			break;
		case PRE::FORMAT_B8G8R8X8_TYPELESS:
			return DXGI_FORMAT_B8G8R8X8_TYPELESS;
			break;
		case PRE::FORMAT_B8G8R8X8_UNORM_SRGB:
			return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
			break;
		case PRE::FORMAT_BC6H_TYPELESS:
			return DXGI_FORMAT_BC6H_TYPELESS;
			break;
		case PRE::FORMAT_BC6H_UF16:
			return DXGI_FORMAT_BC6H_UF16;
			break;
		case PRE::FORMAT_BC6H_SF16:
			return DXGI_FORMAT_BC6H_SF16;
			break;
		case PRE::FORMAT_BC7_TYPELESS:
			return DXGI_FORMAT_BC7_TYPELESS;
			break;
		case PRE::FORMAT_BC7_UNORM:
			return DXGI_FORMAT_BC7_UNORM;
			break;
		case PRE::FORMAT_BC7_UNORM_SRGB:
			return DXGI_FORMAT_BC7_UNORM_SRGB;
			break;
		case PRE::FORMAT_AYUV:
			return DXGI_FORMAT_AYUV;
			break;
		case PRE::FORMAT_Y410:
			return DXGI_FORMAT_Y410;
			break;
		case PRE::FORMAT_Y416:
			return DXGI_FORMAT_Y416;
			break;
		case PRE::FORMAT_NV12:
			return DXGI_FORMAT_NV12;
			break;
		case PRE::FORMAT_P010:
			return DXGI_FORMAT_P010;
			break;
		case PRE::FORMAT_P016:
			return DXGI_FORMAT_P016;
			break;
		case PRE::FORMAT_420_OPAQUE:
			return DXGI_FORMAT_420_OPAQUE;
			break;
		case PRE::FORMAT_YUY2:
			return DXGI_FORMAT_YUY2;
			break;
		case PRE::FORMAT_Y210:
			return DXGI_FORMAT_Y210;
			break;
		case PRE::FORMAT_Y216:
			return DXGI_FORMAT_Y216;
			break;
		case PRE::FORMAT_NV11:
			return DXGI_FORMAT_NV11;
			break;
		case PRE::FORMAT_AI44:
			return DXGI_FORMAT_AI44;
			break;
		case PRE::FORMAT_IA44:
			return DXGI_FORMAT_IA44;
			break;
		case PRE::FORMAT_P8:
			return DXGI_FORMAT_P8;
			break;
		case PRE::FORMAT_A8P8:
			return DXGI_FORMAT_A8P8;
			break;
		case PRE::FORMAT_B4G4R4A4_UNORM:
			return DXGI_FORMAT_B4G4R4A4_UNORM;
			break;
		case PRE::FORMAT_FORCE_UINT:
			return DXGI_FORMAT_FORCE_UINT;
			break;
		default:
			break;
		}
		return DXGI_FORMAT_UNKNOWN;

	}


	inline D3D11_TEXTURE1D_DESC ConvertTextureDesc1D(const TextureDesc* pDesc)
	{
		D3D11_TEXTURE1D_DESC desc;
		desc.Width = pDesc->Width;
		desc.MipLevels = pDesc->MipLevels;
		desc.ArraySize = pDesc->ArraySize;
		desc.Format = ConvertFormat(pDesc->Format);
		desc.Usage = ConvertUsage(pDesc->Usage);
		desc.BindFlags = ParseBindFlags(pDesc->BindFlags);
		desc.CPUAccessFlags = ParseCPUAccessFlags(pDesc->CPUAccessFlags);
		desc.MiscFlags = ParseCPUAccessFlags(pDesc->MiscFlags);
		return desc;
	}

	inline D3D11_TEXTURE2D_DESC ConvertTextureDesc2D(const TextureDesc* pDesc)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = pDesc->Width;
		desc.Height = pDesc->Height;
		desc.MipLevels = pDesc->MipLevels;
		desc.ArraySize = pDesc->ArraySize;
		desc.Format = ConvertFormat(pDesc->Format);
		desc.SampleDesc.Count = pDesc->SampleDesc.Count;
		desc.SampleDesc.Quality = pDesc->SampleDesc.Quality;
		desc.Usage = ConvertUsage(pDesc->Usage);
		desc.BindFlags = ParseBindFlags(pDesc->BindFlags);
		desc.CPUAccessFlags = ParseCPUAccessFlags(pDesc->CPUAccessFlags);
		desc.MiscFlags = ParseResourceMiscFlags(pDesc->MiscFlags);

		return desc;
	}

	inline D3D11_TEXTURE3D_DESC ConvertTextureDesc3D(const TextureDesc* pDesc)
	{
		D3D11_TEXTURE3D_DESC desc;
		desc.Width = pDesc->Width;
		desc.Height = pDesc->Height;
		desc.Depth = pDesc->Depth;
		desc.MipLevels = pDesc->MipLevels;
		desc.Format = ConvertFormat(pDesc->Format);
		desc.Usage = ConvertUsage(pDesc->Usage);
		desc.BindFlags = ParseBindFlags(pDesc->BindFlags);
		desc.CPUAccessFlags = ParseCPUAccessFlags(pDesc->CPUAccessFlags);
		desc.MiscFlags = ParseResourceMiscFlags(pDesc->MiscFlags);
	
		return desc;
	}

	inline D3D11_SUBRESOURCE_DATA ConvertSubresourceData(const SubresourceData& pInitialData)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = pInitialData.pSysMem;
		data.SysMemPitch = pInitialData.SysMemPitch;
		data.SysMemSlicePitch = pInitialData.SysMemSlicePitch;

		return data;
	}
///////////////////////
	/////////////////////
	////////////////////////////////
	inline UINT ParseBindFlags_Inv(UINT value)
	{
		UINT flag = 0;

		if (value & D3D11_BIND_VERTEX_BUFFER)
			flag |= BIND_VERTEX_BUFFER;
		if (value & D3D11_BIND_INDEX_BUFFER)
			flag |= BIND_INDEX_BUFFER;
		if (value & D3D11_BIND_CONSTANT_BUFFER)
			flag |= BIND_CONSTANT_BUFFER;
		if (value & D3D11_BIND_SHADER_RESOURCE)
			flag |= BIND_SHADER_RESOURCE;
		if (value & D3D11_BIND_STREAM_OUTPUT)
			flag |= BIND_STREAM_OUTPUT;
		if (value & D3D11_BIND_RENDER_TARGET)
			flag |= BIND_RENDER_TARGET;
		if (value & D3D11_BIND_DEPTH_STENCIL)
			flag |= BIND_DEPTH_STENCIL;
		if (value & D3D11_BIND_UNORDERED_ACCESS)
			flag |= BIND_UNORDERED_ACCESS;

		return flag;
	}

	inline UINT ParseCPUAccessFlags_Inv(UINT value)
	{
		UINT flag = 0;

		if (value & D3D11_CPU_ACCESS_WRITE)
			flag |= CPU_ACCESS_WRITE;
		if (value & D3D11_CPU_ACCESS_READ)
			flag |= CPU_ACCESS_READ;

		return flag;
	}
	inline UINT ParseResourceMiscFlags_Inv(UINT value)
	{
		UINT flag = 0;

		if (value & D3D11_RESOURCE_MISC_SHARED)
			flag |= RESOURCE_MISC_SHARED;
		if (value & D3D11_RESOURCE_MISC_TEXTURECUBE)
			flag |= RESOURCE_MISC_TEXTURECUBE;
		if (value & D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS)
			flag |= RESOURCE_MISC_DRAWINDIRECT_ARGS;
		if (value & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
			flag |= RESOUCRE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		if (value & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
			flag |= RESOUCRE_MISC_BUFFER_STRUCTURED;
		if (value & D3D11_RESOURCE_MISC_TILED)
			flag |= D3D11_RESOURCE_MISC_TILED;

		return flag;
	}

inline FORMAT ConvertFormat_Inv(DXGI_FORMAT value)
	{
		switch (value)
		{
		case DXGI_FORMAT_UNKNOWN:
			return FORMAT_UNKNOWN;
			break;
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			return FORMAT_R32G32B32A32_TYPELESS;
			break;
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return FORMAT_R32G32B32A32_FLOAT;
			break;
		case DXGI_FORMAT_R32G32B32A32_UINT:
			return FORMAT_R32G32B32A32_UINT;
			break;
		case DXGI_FORMAT_R32G32B32A32_SINT:
			return FORMAT_R32G32B32A32_SINT;
			break;
		case DXGI_FORMAT_R32G32B32_TYPELESS:
			return FORMAT_R32G32B32_TYPELESS;
			break;
		case DXGI_FORMAT_R32G32B32_FLOAT:
			return FORMAT_R32G32B32_FLOAT;
			break;
		case DXGI_FORMAT_R32G32B32_UINT:
			return FORMAT_R32G32B32_UINT;
			break;
		case DXGI_FORMAT_R32G32B32_SINT:
			return FORMAT_R32G32B32_SINT;
			break;
		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
			return FORMAT_R16G16B16A16_TYPELESS;
			break;
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return FORMAT_R16G16B16A16_FLOAT;
			break;
		case DXGI_FORMAT_R16G16B16A16_UNORM:
			return FORMAT_R16G16B16A16_UNORM;
			break;
		case DXGI_FORMAT_R16G16B16A16_UINT:
			return FORMAT_R16G16B16A16_UINT;
			break;
		case DXGI_FORMAT_R16G16B16A16_SNORM:
			return FORMAT_R16G16B16A16_SNORM;
			break;
		case DXGI_FORMAT_R16G16B16A16_SINT:
			return FORMAT_R16G16B16A16_SINT;
			break;
		case DXGI_FORMAT_R32G32_TYPELESS:
			return FORMAT_R32G32_TYPELESS;
			break;
		case DXGI_FORMAT_R32G32_FLOAT:
			return FORMAT_R32G32_FLOAT;
			break;
		case DXGI_FORMAT_R32G32_UINT:
			return FORMAT_R32G32_UINT;
			break;
		case DXGI_FORMAT_R32G32_SINT:
			return FORMAT_R32G32_SINT;
			break;
		case DXGI_FORMAT_R32G8X24_TYPELESS:
			return FORMAT_R32G8X24_TYPELESS;
			break;
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			return FORMAT_D32_FLOAT_S8X24_UINT;
			break;
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			return FORMAT_R32_FLOAT_X8X24_TYPELESS;
			break;
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return FORMAT_X32_TYPELESS_G8X24_UINT;
			break;
		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
			return FORMAT_R10G10B10A2_TYPELESS;
			break;
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			return FORMAT_R10G10B10A2_UNORM;
			break;
		case DXGI_FORMAT_R10G10B10A2_UINT:
			return FORMAT_R10G10B10A2_UINT;
			break;
		case DXGI_FORMAT_R11G11B10_FLOAT:
			return FORMAT_R11G11B10_FLOAT;
			break;
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
			return FORMAT_R8G8B8A8_TYPELESS;
			break;
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return FORMAT_R8G8B8A8_UNORM;
			break;
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return FORMAT_R8G8B8A8_UNORM_SRGB;
			break;
		case DXGI_FORMAT_R8G8B8A8_UINT:
			return FORMAT_R8G8B8A8_UINT;
			break;
		case DXGI_FORMAT_R8G8B8A8_SNORM:
			return FORMAT_R8G8B8A8_SNORM;
			break;
		case DXGI_FORMAT_R8G8B8A8_SINT:
			return FORMAT_R8G8B8A8_SINT;
			break;
		case DXGI_FORMAT_R16G16_TYPELESS:
			return FORMAT_R16G16_TYPELESS;
			break;
		case DXGI_FORMAT_R16G16_FLOAT:
			return FORMAT_R16G16_FLOAT;
			break;
		case DXGI_FORMAT_R16G16_UNORM:
			return FORMAT_R16G16_UNORM;
			break;
		case DXGI_FORMAT_R16G16_UINT:
			return FORMAT_R16G16_UINT;
			break;
		case DXGI_FORMAT_R16G16_SNORM:
			return FORMAT_R16G16_SNORM;
			break;
		case DXGI_FORMAT_R16G16_SINT:
			return FORMAT_R16G16_SINT;
			break;
		case DXGI_FORMAT_R32_TYPELESS:
			return FORMAT_R32_TYPELESS;
			break;
		case DXGI_FORMAT_D32_FLOAT:
			return FORMAT_D32_FLOAT;
			break;
		case DXGI_FORMAT_R32_FLOAT:
			return FORMAT_R32_FLOAT;
			break;
		case DXGI_FORMAT_R32_UINT:
			return FORMAT_R32_UINT;
			break;
		case DXGI_FORMAT_R32_SINT:
			return FORMAT_R32_SINT;
			break;
		case DXGI_FORMAT_R24G8_TYPELESS:
			return FORMAT_R24G8_TYPELESS;
			break;
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			return FORMAT_D24_UNORM_S8_UINT;
			break;
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
			return FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			return FORMAT_X24_TYPELESS_G8_UINT;
			break;
		case DXGI_FORMAT_R8G8_TYPELESS:
			return FORMAT_R8G8_TYPELESS;
			break;
		case DXGI_FORMAT_R8G8_UNORM:
			return FORMAT_R8G8_UNORM;
			break;
		case DXGI_FORMAT_R8G8_UINT:
			return FORMAT_R8G8_UINT;
			break;
		case DXGI_FORMAT_R8G8_SNORM:
			return FORMAT_R8G8_SNORM;
			break;
		case DXGI_FORMAT_R8G8_SINT:
			return FORMAT_R8G8_SINT;
			break;
		case DXGI_FORMAT_R16_TYPELESS:
			return FORMAT_R16_TYPELESS;
			break;
		case DXGI_FORMAT_R16_FLOAT:
			return FORMAT_R16_FLOAT;
			break;
		case DXGI_FORMAT_D16_UNORM:
			return FORMAT_D16_UNORM;
			break;
		case DXGI_FORMAT_R16_UNORM:
			return FORMAT_R16_UNORM;
			break;
		case DXGI_FORMAT_R16_UINT:
			return FORMAT_R16_UINT;
			break;
		case DXGI_FORMAT_R16_SNORM:
			return FORMAT_R16_SNORM;
			break;
		case DXGI_FORMAT_R16_SINT:
			return FORMAT_R16_SINT;
			break;
		case DXGI_FORMAT_R8_TYPELESS:
			return FORMAT_R8_TYPELESS;
			break;
		case DXGI_FORMAT_R8_UNORM:
			return FORMAT_R8_UNORM;
			break;
		case DXGI_FORMAT_R8_UINT:
			return FORMAT_R8_UINT;
			break;
		case DXGI_FORMAT_R8_SNORM:
			return FORMAT_R8_SNORM;
			break;
		case DXGI_FORMAT_R8_SINT:
			return FORMAT_R8_SINT;
			break;
		case DXGI_FORMAT_A8_UNORM:
			return FORMAT_A8_UNORM;
			break;
		case DXGI_FORMAT_R1_UNORM:
			return FORMAT_R1_UNORM;
			break;
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
			return FORMAT_R9G9B9E5_SHAREDEXP;
			break;
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
			return FORMAT_R8G8_B8G8_UNORM;
			break;
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
			return FORMAT_G8R8_G8B8_UNORM;
			break;
		case DXGI_FORMAT_BC1_TYPELESS:
			return FORMAT_BC1_TYPELESS;
			break;
		case DXGI_FORMAT_BC1_UNORM:
			return FORMAT_BC1_UNORM;
			break;
		case DXGI_FORMAT_BC1_UNORM_SRGB:
			return FORMAT_BC1_UNORM_SRGB;
			break;
		case DXGI_FORMAT_BC2_TYPELESS:
			return FORMAT_BC2_TYPELESS;
			break;
		case DXGI_FORMAT_BC2_UNORM:
			return FORMAT_BC2_UNORM;
			break;
		case DXGI_FORMAT_BC2_UNORM_SRGB:
			return FORMAT_BC2_UNORM_SRGB;
			break;
		case DXGI_FORMAT_BC3_TYPELESS:
			return FORMAT_BC3_TYPELESS;
			break;
		case DXGI_FORMAT_BC3_UNORM:
			return FORMAT_BC3_UNORM;
			break;
		case DXGI_FORMAT_BC3_UNORM_SRGB:
			return FORMAT_BC3_UNORM_SRGB;
			break;
		case DXGI_FORMAT_BC4_TYPELESS:
			return FORMAT_BC4_TYPELESS;
			break;
		case DXGI_FORMAT_BC4_UNORM:
			return FORMAT_BC4_UNORM;
			break;
		case DXGI_FORMAT_BC4_SNORM:
			return FORMAT_BC4_SNORM;
			break;
		case DXGI_FORMAT_BC5_TYPELESS:
			return FORMAT_BC5_TYPELESS;
			break;
		case DXGI_FORMAT_BC5_UNORM:
			return FORMAT_BC5_UNORM;
			break;
		case DXGI_FORMAT_BC5_SNORM:
			return FORMAT_BC5_SNORM;
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
			return FORMAT_B5G6R5_UNORM;
			break;
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			return FORMAT_B5G5R5A1_UNORM;
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return FORMAT_B8G8R8A8_UNORM;
			break;
		case DXGI_FORMAT_B8G8R8X8_UNORM:
			return FORMAT_B8G8R8X8_UNORM;
			break;
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			return FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
			break;
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
			return FORMAT_B8G8R8A8_TYPELESS;
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			return FORMAT_B8G8R8A8_UNORM_SRGB;
			break;
		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
			return FORMAT_B8G8R8X8_TYPELESS;
			break;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return FORMAT_B8G8R8X8_UNORM_SRGB;
			break;
		case DXGI_FORMAT_BC6H_TYPELESS:
			return FORMAT_BC6H_TYPELESS;
			break;
		case DXGI_FORMAT_BC6H_UF16:
			return FORMAT_BC6H_UF16;
			break;
		case DXGI_FORMAT_BC6H_SF16:
			return FORMAT_BC6H_SF16;
			break;
		case DXGI_FORMAT_BC7_TYPELESS:
			return FORMAT_BC7_TYPELESS;
			break;
		case DXGI_FORMAT_BC7_UNORM:
			return FORMAT_BC7_UNORM;
			break;
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			return FORMAT_BC7_UNORM_SRGB;
			break;
		case DXGI_FORMAT_AYUV:
			return FORMAT_AYUV;
			break;
		case DXGI_FORMAT_Y410:
			return FORMAT_Y410;
			break;
		case DXGI_FORMAT_Y416:
			return FORMAT_Y416;
			break;
		case DXGI_FORMAT_NV12:
			return FORMAT_NV12;
			break;
		case DXGI_FORMAT_P010:
			return FORMAT_P010;
			break;
		case DXGI_FORMAT_P016:
			return FORMAT_P016;
			break;
		case DXGI_FORMAT_420_OPAQUE:
			return FORMAT_420_OPAQUE;
			break;
		case DXGI_FORMAT_YUY2:
			return FORMAT_YUY2;
			break;
		case DXGI_FORMAT_Y210:
			return FORMAT_Y210;
			break;
		case DXGI_FORMAT_Y216:
			return FORMAT_Y216;
			break;
		case DXGI_FORMAT_NV11:
			return FORMAT_NV11;
			break;
		case DXGI_FORMAT_AI44:
			return FORMAT_AI44;
			break;
		case DXGI_FORMAT_IA44:
			return FORMAT_IA44;
			break;
		case DXGI_FORMAT_P8:
			return FORMAT_P8;
			break;
		case DXGI_FORMAT_A8P8:
			return FORMAT_A8P8;
			break;
		case DXGI_FORMAT_B4G4R4A4_UNORM:
			return FORMAT_B4G4R4A4_UNORM;
			break;
		case DXGI_FORMAT_FORCE_UINT:
			return FORMAT_FORCE_UINT;
			break;
		default:
			break;
		}
		return FORMAT_UNKNOWN;
}
inline USAGE ConvertUsage_Inv(D3D11_USAGE value)
{
	switch (value)
	{
	case D3D11_USAGE_DEFAULT:
		return USAGE_DEFAULT;
		break;
	case D3D11_USAGE_IMMUTABLE:
		return USAGE_IMMUTABLE;
		break;
	case D3D11_USAGE_DYNAMIC:
		return USAGE_DYNAMIC;
		break;
	case D3D11_USAGE_STAGING:
		return USAGE_STAGING;
		break;
	default:
		break;
	}
	return USAGE_DEFAULT;
}

inline TextureDesc ConvertTextureDesc_Inv(const D3D11_TEXTURE1D_DESC* pDesc)
{
	TextureDesc desc;
	desc.Width = pDesc->Width;
	desc.MipLevels = pDesc->MipLevels;
	desc.ArraySize = pDesc->ArraySize;
	desc.Format = ConvertFormat_Inv(pDesc->Format);
	desc.Usage = ConvertUsage_Inv(pDesc->Usage);
	desc.BindFlags = ParseBindFlags_Inv(pDesc->BindFlags);
	desc.CPUAccessFlags = ParseCPUAccessFlags_Inv(pDesc->CPUAccessFlags);
	desc.MiscFlags = ParseResourceMiscFlags_Inv(pDesc->MiscFlags);

	return desc;
}
inline TextureDesc ConvertTextureDesc_Inv(const D3D11_TEXTURE2D_DESC* pDesc)
{
	TextureDesc desc;
	desc.Width = pDesc->Width;
	desc.Height = pDesc->Height;
	desc.MipLevels = pDesc->MipLevels;
	desc.ArraySize = pDesc->ArraySize;
	desc.Format = ConvertFormat_Inv(pDesc->Format);
	desc.SampleDesc.Count = pDesc->SampleDesc.Count;
	desc.SampleDesc.Quality = pDesc->SampleDesc.Quality;
	desc.Usage = ConvertUsage_Inv(pDesc->Usage);
	desc.BindFlags = ParseBindFlags_Inv(pDesc->BindFlags);
	desc.CPUAccessFlags = ParseCPUAccessFlags_Inv(pDesc->CPUAccessFlags);
	desc.MiscFlags = ParseResourceMiscFlags_Inv(pDesc->MiscFlags);

	return desc;
}
inline TextureDesc ConvertTextureDesc_Inv(const D3D11_TEXTURE3D_DESC* pDesc)
{
	TextureDesc desc;
	desc.Width = pDesc->Width;
	desc.Height = pDesc->Height;
	desc.Depth = pDesc->Depth;
	desc.MipLevels = pDesc->MipLevels;
	desc.Format = ConvertFormat_Inv(pDesc->Format);
	desc.Usage = ConvertUsage_Inv(pDesc->Usage);
	desc.BindFlags = ParseBindFlags_Inv(pDesc->BindFlags);
	desc.CPUAccessFlags = ParseCPUAccessFlags_Inv(pDesc->CPUAccessFlags);
	desc.MiscFlags = ParseResourceMiscFlags_Inv(pDesc->MiscFlags);

	return desc;
}


const void* const nullBlob[1024] = {};




RenderDevice_DX11::RenderDevice_DX11(HWND mainscreen,bool fullscreen, bool debuglayer):RenderDevice()
{
	FULLSCREEN = fullscreen;

	

	RECT rect = RECT();
	GetClientRect(mainscreen, &rect);

	SCREENWIDTH = rect.right-rect.left;
	SCREENHEIGHT = rect.bottom - rect.top;



	HRESULT hr = E_FAIL;
	for (int i=0;i<JobScheduler::NumWorker;i++)
	{
		SAFE_INITIL(commandlists[i]);
		SAFE_INITIL(deviceContexts[i]);
		stencilRef[i] = 0;
		blendFactor[i] = XMFLOAT4(1, 1, 1, 1);
	}

	UINT createDeviceFlags = 0;

	if (debuglayer)
	{
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}

	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0 ,D3D_FEATURE_LEVEL_10_1 };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex=0;driverTypeIndex<numDriverTypes;driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &Basedevice, &featureLevel, &BasedeviceContexts);
	
		if(SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
	{
		MessageBox(NULL,
			_T("Create Device failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
	}

	IDXGIFactory3* DXGIFactory = nullptr;
	{
		IDXGIDevice3* DXGIDevice = nullptr;
	     HRESULT hr = Basedevice->QueryInterface(__uuidof(IDXGIDevice3), reinterpret_cast<void**>(&DXGIDevice));
	
		if (SUCCEEDED(hr))
		{
			DXGIDevice->SetMaximumFrameLatency(1);
			IDXGIAdapter* adapter = nullptr;
			HRESULT	hr1 = DXGIDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr1))
			{
				hr1 =adapter->GetParent(__uuidof(IDXGIFactory3), reinterpret_cast<void**>(&DXGIFactory));
				SAFE_RELEASE(adapter);
			}
			SAFE_RELEASE(DXGIDevice);
		}
	}

	 hr = DXGIFactory->QueryInterface(__uuidof(IDXGIFactory3), reinterpret_cast<void**>(&DXGIFactory));
	if (DXGIFactory)
	{
		hr = Basedevice->QueryInterface(__uuidof(ID3D11Device3), reinterpret_cast<void**>(&device));
		if (SUCCEEDED(hr))
		{
		   hr=BasedeviceContexts->QueryInterface(__uuidof(ID3D11DeviceContext3), reinterpret_cast<void**>(&ImmediatedeviceContext));
		}
	}


	DXGI_SWAP_CHAIN_DESC1 sd = { 0 };
	sd.Width = SCREENWIDTH;
	sd.Height = SCREENHEIGHT;
	sd.Format = ConvertFormat(GetBackBufferFormat());
	sd.Stereo = false;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.Flags = 0;
	sd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;


	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	sd.Scaling = DXGI_SCALING_STRETCH;
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
	fullscreenDesc.RefreshRate.Numerator = 60;
	fullscreenDesc.RefreshRate.Denominator = 1;
	fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // needs to be unspecified for correct fullscreen scaling!/////////
	fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	fullscreenDesc.Windowed = !fullscreen;
	hr = DXGIFactory->CreateSwapChainForHwnd(device, mainscreen, &sd, &fullscreenDesc, nullptr, &swapChain);
	
	if (FAILED(hr))
	{
		MessageBox(NULL,
			_T("Create SwapChain failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		SAFE_RELEASE(DXGIFactory);
	
     }
	SAFE_RELEASE(DXGIFactory);
	         
	

	hr = ImmediatedeviceContext->QueryInterface(__uuidof(userDefinedAnnotations[0]),
		reinterpret_cast<void**>(&userDefinedAnnotations[0]));
	D3D_FEATURE_LEVEL aquiredFeatureLevel = device->GetFeatureLevel();
	TESSELLATION = ((aquiredFeatureLevel > D3D_FEATURE_LEVEL_11_0) ? true : false);

	D3D11_FEATURE_DATA_D3D11_OPTIONS2 features_2;
	hr = device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS2, &features_2, sizeof(features_2));//
	CONSERVATIVE_RASTERIZATION = features_2.ConservativeRasterizationTier >= D3D11_CONSERVATIVE_RASTERIZATION_TIER_1;
	RASTERIZER_ORDERED_VIEWS = features_2.ROVsSupported == TRUE;
	UNORDEREDACCESSTEXTURE_LOAD_EXT = features_2.TypedUAVLoadAdditionalFormats == TRUE;

	D3D11_FEATURE_DATA_THREADING threadingFeature;
	device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadingFeature, sizeof(threadingFeature));
	if (threadingFeature.DriverConcurrentCreates && threadingFeature.DriverCommandLists)
	{
		MULTITHREAD_RENDERING = true;
		for (int i=0;i<JobScheduler::NumWorker;i++)
		{
 				hr = device->CreateDeferredContext3(0, &deviceContexts[i]);
 				//hr = deviceContexts[i]->QueryInterface(__uuidof(userDefinedAnnotations[i]), reinterpret_cast<void**>(&userDefinedAnnotations[i]));
		}
	}
	else
	{
		MULTITHREAD_RENDERING = false;
	}


	CreateRenderTargetAndDepthStencil();

	

}

RenderDevice_DX11::~RenderDevice_DX11()
{


    SAFE_RELEASE(renderTargetView);
    SAFE_RELEASE(swapChain);

	for (int i=0;i<JobScheduler::NumWorker;i++)
	{
		SAFE_RELEASE(commandlists[i]);
	}
	SAFE_RELEASE(BasedeviceContexts);
	SAFE_DELETE_ARRAY(*deviceContexts);
	ID3D11Debug* d3dDebug;
	HRESULT hr = device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	if (d3dDebug != nullptr)
		d3dDebug->Release();
	SAFE_RELEASE(device);
	SAFE_RELEASE(Basedevice);
}

void RenderDevice_DX11::SetResolution(int width, int height)
{
	D3D11_VIEWPORT ScreenViewport;
	ScreenViewport.TopLeftX = 0;
	ScreenViewport.TopLeftY = 0;
	ScreenViewport.Width = static_cast<float>(width);
	ScreenViewport.Height = static_cast<float>(height);

	ScreenViewport.MinDepth = 0.0f;
	ScreenViewport.MaxDepth = 1.0f;

	for (int i = 0; i < 8; i++)
	{
		deviceContexts[i]->RSSetViewports(1, &ScreenViewport);
	}

	if (height != SCREENHEIGHT || width != SCREENWIDTH)
	{
		SCREENHEIGHT = height;
		SCREENWIDTH = width;

		SAFE_RELEASE(backbuffer);
		SAFE_RELEASE(renderTargetView);
		HRESULT hr = swapChain->ResizeBuffers(GetBackBufferCount(), width, height, ConvertFormat(GetBackBufferFormat()), 0);
		assert(SUCCEEDED(hr));
		CreateRenderTargetAndDepthStencil();
	}
	
	

	for (int i = 0; i < 8; i++)
	{
		deviceContexts[i]->OMSetRenderTargets(1, &renderTargetView, DepthStecilView);
	}
}
	
Texture2D RenderDevice_DX11::GetBackBuffer()
{
	Texture2D result;
	result.resource = (CPUHandle)backbuffer;
	backbuffer->AddRef();

	D3D11_TEXTURE2D_DESC desc;
	backbuffer->GetDesc(&desc);
	result.desc = ConvertTextureDesc_Inv(&desc);

	return result;
}



HRESULT RenderDevice_DX11::CreateBuffer(const GPUBufferDesc *pDesc, const SubresourceData* pInitialData, GPUBuffer *ppBuffer)
{
	ppBuffer->Register(this);
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = pDesc->ByteWidth;
	desc.Usage = ConvertUsage(pDesc->Usage);
	desc.BindFlags = ParseBindFlags(pDesc->BindFlags);
	desc.CPUAccessFlags = ParseCPUAccessFlags(pDesc->CPUAccessFlags);
	desc.MiscFlags = ParseResourceMiscFlags(pDesc->MiscFlags);
	desc.StructureByteStride = pDesc->StructureByteStride;

	D3D11_SUBRESOURCE_DATA* data = nullptr;
	if (pInitialData!=nullptr)
	{
		data = new D3D11_SUBRESOURCE_DATA[1];
		for (UINT slice=0;slice<1;++slice)
		{
			data[slice] = ConvertSubresourceData(pInitialData[slice]);
		}
	}

	ppBuffer->desc = *pDesc;
	HRESULT hr = device->CreateBuffer(&desc, data,(ID3D11Buffer**)&ppBuffer->resource);
	SAFE_DELETE_ARRAY(data);
	assert(SUCCEEDED(hr) && "GPUBuffer creation failed!");
	
	if (SUCCEEDED(hr))
	{
		if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
			ZeroMemory(&srv_desc, sizeof(srv_desc));

			if (desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
			{
				srv_desc.Format = DXGI_FORMAT_R32_TYPELESS;
				srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				srv_desc.BufferEx.FirstElement = 0;
				srv_desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
				srv_desc.BufferEx.NumElements = desc.ByteWidth / 4;
			}
			else if (desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
			{
				srv_desc.Format = DXGI_FORMAT_UNKNOWN;
				srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				srv_desc.BufferEx.FirstElement = 0;
				srv_desc.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
			}
			else
			{
				srv_desc.Format = ConvertFormat(pDesc->Format);
				srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				srv_desc.Buffer.FirstElement = 0;
				srv_desc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
			}
			hr = device->CreateShaderResourceView((ID3D11Resource*)ppBuffer->resource, &srv_desc, (ID3D11ShaderResourceView**)&ppBuffer->SRV);

			assert(SUCCEEDED(hr) && "ShaderResourceView of the GPUBuffer could not be created!");
		}
		if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			ZeroMemory(&uav_desc, sizeof(uav_desc));
			uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uav_desc.Buffer.FirstElement = 0;

			if (desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
			{
				uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
				uav_desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
				uav_desc.Buffer.NumElements = desc.ByteWidth / 4;
			}
			else if (desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
			{
				uav_desc.Format = DXGI_FORMAT_UNKNOWN;
				uav_desc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
			}
			else
			{
				uav_desc.Format = ConvertFormat(pDesc->Format);
				uav_desc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
			}

			hr = device->CreateUnorderedAccessView((ID3D11Resource*)ppBuffer->resource, &uav_desc, (ID3D11UnorderedAccessView**)&ppBuffer->UAV);
			assert(SUCCEEDED(hr) && "UnorderedAccessView of the GPUBuffer can not be created!");
		}
	}
	return hr;
}

HRESULT RenderDevice_DX11::CreateTexture1D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture1D **ppTexture1D)
{
     if ((*ppTexture1D)==nullptr)
     {
		 (*ppTexture1D) = new Texture1D;
     }
	 (*ppTexture1D)->Register(this);

	 (*ppTexture1D)->desc = *pDesc;

	 D3D11_TEXTURE1D_DESC desc = ConvertTextureDesc1D(&(*ppTexture1D)->desc);

	 D3D11_SUBRESOURCE_DATA* data = nullptr;
	 if (pInitialData!=nullptr)
	 {
		 data = new D3D11_SUBRESOURCE_DATA[pDesc->ArraySize];
		 for (UINT slice=0;slice<pDesc->ArraySize;++slice)
		 {
			 data[slice] = ConvertSubresourceData(pInitialData[slice]);
		 }
	 }

	 HRESULT hr = S_OK;
	 hr = device->CreateTexture1D(&desc, data, (ID3D11Texture1D**)&((*ppTexture1D)->resource));
	 SAFE_DELETE_ARRAY(data);
	assert(SUCCEEDED(hr) && "Texture1D creation failed!");
	 if (FAILED(hr))
		 return hr;

	 if ((*ppTexture1D)->desc.MipLevels==0)
	 {
	    (*ppTexture1D)->desc.MipLevels = (UINT)log2((*ppTexture1D)->desc.Width);
	 }

	 CreateShaderResourceView(*ppTexture1D);
	 if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	 {
		 assert((*ppTexture1D)->independentRTVArraySlices == false && "TextureArray UAV not implemnted");

		 D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
		 ZeroMemory(&uav_desc, sizeof(uav_desc));
		 uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
		 uav_desc.Texture2D.MipSlice = 0;

		 hr = device->CreateUnorderedAccessView((ID3D11Resource*)(*ppTexture1D)->resource, &uav_desc, (ID3D11UnorderedAccessView**)&(*ppTexture1D)->UAV);
		 assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture1D could not be created!");
	 }

	 return hr;
}

HRESULT RenderDevice_DX11::CreateTexture2D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture2D **ppTexture2D)
{

	if ((*ppTexture2D) == nullptr)
	{
		(*ppTexture2D) = new Texture2D;
	}
	(*ppTexture2D)->Register(this);

	(*ppTexture2D)->desc = *pDesc;
	
	if ((*ppTexture2D)->desc.SampleDesc.Count>1)
	{
		UINT qulity;
		device->CheckMultisampleQualityLevels(ConvertFormat((*ppTexture2D)->desc.Format),(*ppTexture2D)->desc.SampleDesc.Count,&qulity);
		(*ppTexture2D)->desc.SampleDesc.Quality = qulity - 1;
		if (qulity == 0)
		{
			assert(0 && "MSAA Samplecount not supported!");
			(*ppTexture2D)->desc.SampleDesc.Count = 1;
		}
	}

	D3D11_TEXTURE2D_DESC desc = ConvertTextureDesc2D(&(*ppTexture2D)->desc);


	D3D11_SUBRESOURCE_DATA* data = nullptr;
	if (pInitialData != nullptr)
	{
		UINT dataCount = pDesc->ArraySize* max(1, pDesc->MipLevels);
		data = new D3D11_SUBRESOURCE_DATA[dataCount];
		for (UINT slice=0;slice<dataCount;++slice)
		{
			data[slice] = ConvertSubresourceData(pInitialData[slice]);
		}
	}

	HRESULT hr = S_OK;

	hr = device->CreateTexture2D(&desc, data, (ID3D11Texture2D**)&((*ppTexture2D)->resource));
	assert(SUCCEEDED(hr) && "Texture2D creation failed!");
	SAFE_DELETE_ARRAY(data);
	if (FAILED(hr))
		return hr;
	 
	if ((*ppTexture2D)->desc.MipLevels==0)
	{
		(*ppTexture2D)->desc.MipLevels = (UINT)log2(max((*ppTexture2D)->desc.Width, (*ppTexture2D)->desc.Height));
	}

 	CreateRenderTargetView(*ppTexture2D);
	CreateShaderResourceView(*ppTexture2D);
	CreateDepthStencilView(*ppTexture2D);

	if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		if (desc.ArraySize > 1)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			ZeroMemory(&uav_desc, sizeof(uav_desc));
			uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
			uav_desc.Texture2DArray.FirstArraySlice = 0;
			uav_desc.Texture2DArray.ArraySize = desc.ArraySize;
			uav_desc.Texture2DArray.MipSlice = 0;

			if ((*ppTexture2D)->indepentdentUAVMIPs)
			{
				UINT miplevels = (*ppTexture2D)->desc.MipLevels;
				for (UINT i=0;i<miplevels;++i)
				{
					uav_desc.Texture2DArray.MipSlice = i;

					(*ppTexture2D)->additionalUAVs.push_back(NUll_Handle);
					hr = device->CreateUnorderedAccessView((ID3D11Resource*)(*ppTexture2D)->resource, &uav_desc, (ID3D11UnorderedAccessView**)&(*ppTexture2D)->additionalUAVs[i]);
					assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture2D could not be created!");
				}
			}
			{
				uav_desc.Texture2DArray.MipSlice = 0;
				hr = device->CreateUnorderedAccessView((ID3D11Resource*)(*ppTexture2D)->resource,&uav_desc, (ID3D11UnorderedAccessView**)&(*ppTexture2D)->UAV);
			}
		}
		else
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			ZeroMemory(&uav_desc, sizeof(uav_desc));
			uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

			if ((*ppTexture2D)->indepentdentUAVMIPs)
			{
				UINT miplevels = (*ppTexture2D)->desc.MipLevels;
				for (UINT i=0;i<miplevels;++i)
				{
					uav_desc.Texture2D.MipSlice = i;

					(*ppTexture2D)->additionalUAVs.push_back(NUll_Handle);
					hr = device->CreateUnorderedAccessView((ID3D11Resource*)(*ppTexture2D)->resource, &uav_desc, (ID3D11UnorderedAccessView**)&(*ppTexture2D)->additionalUAVs[i]);
					assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture2D could not be created!");
				}
			}
			{
				uav_desc.Texture2D.MipSlice = 0;
				hr = device->CreateUnorderedAccessView((ID3D11Resource*)(*ppTexture2D)->resource,&uav_desc, (ID3D11UnorderedAccessView**)&(*ppTexture2D)->UAV);
			}
		}
		assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture2D could not be created!");
	}
	return hr;
}


HRESULT RenderDevice_DX11::CreateTexture3D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture3D **ppTexture3D)
{
	if ((*ppTexture3D) == nullptr)
	{
		(*ppTexture3D) = new Texture3D;
	}
	(*ppTexture3D)->Register(this);

	(*ppTexture3D)->desc = *pDesc;

	D3D11_TEXTURE3D_DESC desc = ConvertTextureDesc3D(&(*ppTexture3D)->desc);

	D3D11_SUBRESOURCE_DATA* data = nullptr;
	if (pInitialData != nullptr)
	{
		data = new D3D11_SUBRESOURCE_DATA[1];
		for (UINT slice = 0; slice < 1; ++slice)
		{
			data[slice] = ConvertSubresourceData(pInitialData[slice]);
		}
	}

	HRESULT hr = S_OK;

	hr = device->CreateTexture3D(&desc, data, (ID3D11Texture3D**)&((*ppTexture3D)->resource));
	SAFE_DELETE_ARRAY(data);
	assert(SUCCEEDED(hr) && "Textture3D creation failed");
	if (FAILED(hr))
	{
		return hr;
	}

	if ((*ppTexture3D)->desc.MipLevels == 0)
	{
		(*ppTexture3D)->desc.MipLevels = (UINT)log2(max((*ppTexture3D)->desc.Width, max((*ppTexture3D)->desc.Height, (*ppTexture3D)->desc.Depth)));
	}

	CreateShaderResourceView(*ppTexture3D);
	CreateRenderTargetView(*ppTexture3D);

	if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
		ZeroMemory(&uav_desc, sizeof(uav_desc));
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
		uav_desc.Texture3D.FirstWSlice = 0;

		if ((*ppTexture3D)->indepentdentUAVMIPs)
		{
			UINT miplevels = (*ppTexture3D)->desc.MipLevels;
			uav_desc.Texture3D.WSize = desc.Depth;
			for (UINT i = 0; i < miplevels; ++i)
			{
				uav_desc.Texture3D.MipSlice = i;

				(*ppTexture3D)->additionalUAVs.push_back(NUll_Handle);
				hr = device->CreateUnorderedAccessView((ID3D11Resource*)(*ppTexture3D)->resource, &uav_desc, (ID3D11UnorderedAccessView**)&(*ppTexture3D)->additionalUAVs[i]);
				assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture3D could not be created!");
				uav_desc.Texture3D.WSize /= 2;
			}
		}

		{
			uav_desc.Texture3D.MipSlice = 0;
			uav_desc.Texture3D.WSize = desc.Depth;
			hr = device->CreateUnorderedAccessView((ID3D11Resource*)(*ppTexture3D)->resource, &uav_desc, (ID3D11UnorderedAccessView**)&(*ppTexture3D)->UAV);
			assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture3D could not be created!");
		}
	}
	return hr;
 }

HRESULT RenderDevice_DX11::CreateShaderResourceView(Texture1D* pTexture)
{
	if (pTexture->SRV != NUll_Handle)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	if (pTexture->desc.BindFlags&BIND_SHADER_RESOURCE)
	{
		UINT arraySize = pTexture->desc.ArraySize;

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = ConvertFormat(pTexture->desc.Format);

		if (arraySize>1)
		{
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
			shaderResourceViewDesc.Texture1DArray.FirstArraySlice = 0;
			shaderResourceViewDesc.Texture1DArray.ArraySize = arraySize;
			shaderResourceViewDesc.Texture1DArray.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture1DArray.MipLevels = -1;
		}
		else
		{
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			shaderResourceViewDesc.Texture1D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture1D.MipLevels = -1;
		}

		hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->SRV);

		assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
	}
	return hr;
}
   
HRESULT RenderDevice_DX11::CreateShaderResourceView(Texture2D* pTexture)
{
     if (pTexture->SRV!=NUll_Handle)
     {
		 return E_FAIL;
     }

	 HRESULT hr = E_FAIL;
	 if (pTexture->desc.BindFlags & BIND_SHADER_RESOURCE)
	 {
		 UINT arraysize = pTexture->desc.ArraySize;
		 UINT sampleCount = pTexture->desc.SampleDesc.Count;
		 bool multisampled = sampleCount > 1;

		 D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		 ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

		 switch (pTexture->desc.Format)
		 {
		 case FORMAT_R16_TYPELESS:
			 shaderResourceViewDesc.Format = DXGI_FORMAT_R16_UNORM;
			 break;
		 case FORMAT_R32_TYPELESS:
			 shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
			 break;
		 case FORMAT_R24G8_TYPELESS:
			 shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			 break;
		 case FORMAT_R32G8X24_TYPELESS:
			 shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			 break;
		 default:
			 shaderResourceViewDesc.Format = ConvertFormat(pTexture->desc.Format);
			 break;
		 }
		 if (arraysize > 1)
		 {
			 if (pTexture->desc.MiscFlags & RESOURCE_MISC_TEXTURECUBE)
			 {
				 if (arraysize>6)
				 {
					 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
					 shaderResourceViewDesc.TextureCubeArray.First2DArrayFace = 0;
					 shaderResourceViewDesc.TextureCubeArray.NumCubes = arraysize;
					 shaderResourceViewDesc.TextureCubeArray.MostDetailedMip = 0;
					 shaderResourceViewDesc.TextureCubeArray.MipLevels = -1;
				 }
				 else
				 {
					 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
					 shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
					 shaderResourceViewDesc.TextureCube.MipLevels = -1;
				 }
			 }
			 else
			 {
				 if (multisampled)
				 {
					 shaderResourceViewDesc.ViewDimension= D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
					 shaderResourceViewDesc.Texture2DMSArray.FirstArraySlice = 0;
					 shaderResourceViewDesc.Texture2DMSArray.ArraySize = arraysize;
				 }
				 else
				 {
					 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
					 shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
					 shaderResourceViewDesc.Texture2DArray.ArraySize = arraysize;
					 shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
					 shaderResourceViewDesc.Texture2DArray.MipLevels = -1;
				 }
			 }
			 hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->SRV);
			 assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");

			 if (pTexture->indepentdentSRVMIPs)
			 {
				 if (pTexture->desc.MiscFlags & RESOURCE_MISC_TEXTURECUBE)
				 {
					 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
					 shaderResourceViewDesc.TextureCubeArray.First2DArrayFace = 0;
					 shaderResourceViewDesc.TextureCubeArray.NumCubes = arraysize / 6;

					 for (UINT j=0;j<pTexture->desc.MipLevels;++j)
					 {
						 shaderResourceViewDesc.TextureCubeArray.MostDetailedMip = j;
						 shaderResourceViewDesc.TextureCubeArray.MipLevels = 1;

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->additionalSRVs.back());
						 assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
					 }
				 }
				 else
				 {
					 UINT slices = arraysize;

					 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					 shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
					 shaderResourceViewDesc.Texture2DArray.ArraySize = pTexture->desc.ArraySize;

					 for (UINT j = 0; j < pTexture->desc.MipLevels; ++j)
					 {
						 shaderResourceViewDesc.Texture2DArray.MostDetailedMip = j;
						 shaderResourceViewDesc.Texture2DArray.MipLevels = 1;

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->additionalSRVs.back());
						 assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
					 }
				 }
			 }

			 if (pTexture->independentSRVArraySlices)
			 {
				 if (pTexture->desc.MiscFlags & RESOURCE_MISC_TEXTURECUBE)
				 {
					 UINT slice = arraysize / 6;

					 for (UINT i=0;i<slice;++i)
					 {
						 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
						 shaderResourceViewDesc.TextureCubeArray.First2DArrayFace = i * 6;
						 shaderResourceViewDesc.TextureCubeArray.NumCubes = 1;
						 shaderResourceViewDesc.TextureCubeArray.MostDetailedMip = 0;
						 shaderResourceViewDesc.TextureCubeArray.MipLevels = -1;

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource,&shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->additionalSRVs.back());
						 assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
					 }
				 }
				 else
				 {
					 UINT slices = arraysize;

					 for (UINT i=0;i<slices;i++)
					 {
						 if (multisampled)
						 {
							 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
							 shaderResourceViewDesc.Texture2DMSArray.FirstArraySlice = i;
							 shaderResourceViewDesc.Texture2DMSArray.ArraySize = 1;
						 }
						 else
						 {
							 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
							 shaderResourceViewDesc.Texture2DArray.FirstArraySlice = i;
							 shaderResourceViewDesc.Texture2DArray.ArraySize = 1;
							 shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
							 shaderResourceViewDesc.Texture2DArray.MipLevels = -1;
						 }

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource,&shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->additionalSRVs.back());
						assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");

					 }
				 }
			 }
		 }
		 else
		 {
			 if (multisampled)
			 {
				 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				 hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->SRV);
				 assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
			 }
			 else
			 {
				 shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				 if (pTexture->indepentdentSRVMIPs)
				 {
					 UINT miplevels = pTexture->desc.MipLevels;
					 for (UINT i = 0; i < miplevels; ++i)
					 {
						 shaderResourceViewDesc.Texture2D.MostDetailedMip = i;
						 shaderResourceViewDesc.Texture2D.MipLevels = 1;

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->additionalSRVs.back());
						 assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
					 }
				 }
				 {
					 shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
					 shaderResourceViewDesc.Texture2D.MipLevels = -1;
					 hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->SRV);
					 assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
				 }	
			 }
		 }
	 }
	 return hr;
}


HRESULT RenderDevice_DX11::CreateShaderResourceView(Texture3D* pTexture)
{
	if (pTexture->SRV != NUll_Handle)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	if (pTexture->desc.BindFlags & BIND_SHADER_RESOURCE)
	{

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = ConvertFormat(pTexture->desc.Format);
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;

		if (pTexture->indepentdentSRVMIPs)
		{
			UINT miplevels = pTexture->desc.MipLevels;
			for (UINT i = 0; i < miplevels; ++i)
			{
				shaderResourceViewDesc.Texture3D.MostDetailedMip = i;
				shaderResourceViewDesc.Texture3D.MipLevels = 1;

				pTexture->additionalSRVs.push_back(NUll_Handle);
				hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->additionalSRVs[i]);
				assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
			}
		}

		{
			
			shaderResourceViewDesc.Texture3D.MostDetailedMip = 0; 
			shaderResourceViewDesc.Texture3D.MipLevels = -1; 

			hr = device->CreateShaderResourceView((ID3D11Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D11ShaderResourceView**)&pTexture->SRV);
			assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
		}

	}

	return hr;
}

HRESULT RenderDevice_DX11::CreateRenderTargetView(Texture2D* pTexture)
{
	if (!pTexture->additionalRTVs.empty())
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	if (pTexture->desc.BindFlags & BIND_RENDER_TARGET)
	{
		UINT arraySize = pTexture->desc.ArraySize;
		UINT sampleCount = pTexture->desc.SampleDesc.Count;
		bool multisampled = sampleCount > 1;

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = ConvertFormat(pTexture->desc.Format);
		renderTargetViewDesc.Texture2DArray.MipSlice = 0;

		if (pTexture->desc.MiscFlags & RESOURCE_MISC_TEXTURECUBE)
		{
			
			UINT slices = arraySize / 6;

			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			renderTargetViewDesc.Texture2DArray.MipSlice = 0;

			if (pTexture->independentRTVCubemapFaces)
			{
				
				for (UINT i = 0; i < arraySize; ++i)
				{
					renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
					renderTargetViewDesc.Texture2DArray.ArraySize = 1;

					pTexture->additionalRTVs.push_back(NUll_Handle);
					hr = device->CreateRenderTargetView((ID3D11Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D11RenderTargetView**)&pTexture->additionalRTVs[i]);
					assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
				}
			}
			else if (pTexture->independentRTVArraySlices)
			{
				
				for (UINT i = 0; i < slices; ++i)
				{
					renderTargetViewDesc.Texture2DArray.FirstArraySlice = i * 6;
					renderTargetViewDesc.Texture2DArray.ArraySize = 6;

					pTexture->additionalRTVs.push_back(NUll_Handle);
					hr = device->CreateRenderTargetView((ID3D11Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D11RenderTargetView**)&pTexture->additionalRTVs[i]);
					assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
				}
			}

			{
				
				renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
				renderTargetViewDesc.Texture2DArray.ArraySize = arraySize;

				hr = device->CreateRenderTargetView((ID3D11Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D11RenderTargetView**)&pTexture->RTV);
				assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
			}
		}
		else
		{
			
			if (arraySize > 1 && pTexture->independentRTVArraySlices)
			{
				
				for (UINT i = 0; i < arraySize; ++i)
				{
					if (multisampled)
					{
						renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
						renderTargetViewDesc.Texture2DMSArray.FirstArraySlice = i;
						renderTargetViewDesc.Texture2DMSArray.ArraySize = 1;
					}
					else
					{
						renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
						renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
						renderTargetViewDesc.Texture2DArray.ArraySize = 1;
						renderTargetViewDesc.Texture2DArray.MipSlice = 0;
					}

					pTexture->additionalRTVs.push_back(NUll_Handle);
					hr = device->CreateRenderTargetView((ID3D11Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D11RenderTargetView**)&pTexture->additionalRTVs[i]);
					assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
				}
			}

			{
				
				if (arraySize > 1)
				{
					if (multisampled)
					{
						renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
						renderTargetViewDesc.Texture2DMSArray.FirstArraySlice = 0;
						renderTargetViewDesc.Texture2DMSArray.ArraySize = arraySize;
					}
					else
					{
						renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
						renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
						renderTargetViewDesc.Texture2DArray.ArraySize = arraySize;
						renderTargetViewDesc.Texture2DArray.MipSlice = 0;
					}
				}
				else
				{
					if (multisampled)
					{
						renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
					}
					else
					{
						renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
						renderTargetViewDesc.Texture2D.MipSlice = 0;
					}
				}

				hr = device->CreateRenderTargetView((ID3D11Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D11RenderTargetView**)&pTexture->RTV);
				assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
			}
		}
	}

	return hr;
}

HRESULT RenderDevice_DX11::CreateRenderTargetView(Texture3D* pTexture)
{
	if (!pTexture->additionalRTVs.empty())
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	if (pTexture->desc.BindFlags & BIND_RENDER_TARGET)
	{
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = ConvertFormat(pTexture->desc.Format);

		if (pTexture->independentRTVArraySlices)
		{
			for (UINT i=0;i<pTexture->GetDesc().Depth;++i)
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
				renderTargetViewDesc.Texture3D.MipSlice = 0;
				renderTargetViewDesc.Texture3D.FirstWSlice = i;
				renderTargetViewDesc.Texture3D.WSize = 1;

				pTexture->additionalRTVs.push_back(NUll_Handle);
				hr = device->CreateRenderTargetView((ID3D11Resource*)pTexture->resource,&renderTargetViewDesc, (ID3D11RenderTargetView**)&pTexture->additionalRTVs[i]);
			}
		}

		{
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
			renderTargetViewDesc.Texture3D.MipSlice = 0;
			renderTargetViewDesc.Texture3D.FirstWSlice = 0;
			renderTargetViewDesc.Texture3D.WSize = -1;

			pTexture->additionalRTVs.push_back(NUll_Handle);
			hr = device->CreateRenderTargetView((ID3D11Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D11RenderTargetView**)&pTexture->additionalRTVs[0]);
		}
		assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
	}

	return hr;	
}

HRESULT RenderDevice_DX11::CreateDepthStencilView(Texture2D* pTexture)
{
    if (!pTexture->additionalDSVs.empty())
    {
		return E_FAIL;
    }

	HRESULT hr = E_FAIL;
	if (pTexture->desc.BindFlags & BIND_DEPTH_STENCIL)
	{
		UINT arraysize = pTexture->desc.ArraySize;
		UINT SampleDesc = pTexture->desc.SampleDesc.Count;
		bool multisample = SampleDesc > 1;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Texture2DArray.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;


		switch (pTexture->desc.Format)
		{
		case FORMAT_R16_TYPELESS:
			depthStencilViewDesc.Format = DXGI_FORMAT_D16_UNORM;
			break;
		case FORMAT_R32_TYPELESS:
			depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
			break;
		case FORMAT_R24G8_TYPELESS:
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		case  FORMAT_R32G8X24_TYPELESS:
			depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			break;
		default:
			depthStencilViewDesc.Format = ConvertFormat(pTexture->desc.Format);
			break;
		}

		if (pTexture->desc.MiscFlags & RESOURCE_MISC_TEXTURECUBE)
		{
			UINT slices = arraysize / 6;

			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			depthStencilViewDesc.Texture2DArray.MipSlice = 0;

			if (pTexture->independentRTVCubemapFaces)
			{
				for (UINT i=0;i<arraysize;++i)
				{
					depthStencilViewDesc.Texture2DArray.FirstArraySlice = i;
					depthStencilViewDesc.Texture2DArray.ArraySize = 1;

					pTexture->additionalDSVs.push_back(NUll_Handle);
					hr = device->CreateDepthStencilView((ID3D11Resource*)pTexture->resource, &depthStencilViewDesc, (ID3D11DepthStencilView**)&pTexture->additionalDSVs[i]);
				}
			}
			else if(pTexture->independentRTVArraySlices)
			{
				for (UINT i=0;i<slices;++i)
				{
					depthStencilViewDesc.Texture2DArray.FirstArraySlice = i * 6;
					depthStencilViewDesc.Texture2DArray.ArraySize = 6;

					pTexture->additionalDSVs.push_back(NUll_Handle);
					hr = device->CreateDepthStencilView((ID3D11Resource*)pTexture->resource,&depthStencilViewDesc, (ID3D11DepthStencilView**)&pTexture->additionalDSVs[i]);
				}
			}
			{
				depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
				depthStencilViewDesc.Texture2DArray.ArraySize = arraysize;
			
				hr = device->CreateDepthStencilView((ID3D11Resource*)pTexture->resource,&depthStencilViewDesc, (ID3D11DepthStencilView**)&pTexture->DSV);
			}
		}
		else
		{
			if (arraysize>1 && pTexture->independentRTVArraySlices)
			{
				for (UINT i=0;i<arraysize;++i)
				{
					if (multisample)
					{
						depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
						depthStencilViewDesc.Texture2DMSArray.FirstArraySlice = i;
						depthStencilViewDesc.Texture2DMSArray.ArraySize = 1;
					}
					else
					{
						depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
						depthStencilViewDesc.Texture2DArray.MipSlice = 0;
						depthStencilViewDesc.Texture2DArray.FirstArraySlice = i;
						depthStencilViewDesc.Texture1DArray.ArraySize = 1;
					}

					pTexture->additionalDSVs.push_back(NUll_Handle);
					hr = device->CreateDepthStencilView((ID3D11Resource*)pTexture->resource, &depthStencilViewDesc, (ID3D11DepthStencilView**)&pTexture->additionalDSVs[i]);
				}
			}
			else
			{
				if (arraysize>1)
				{
					if (multisample)
					{
						depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
						depthStencilViewDesc.Texture2DMSArray.FirstArraySlice = 0;
						depthStencilViewDesc.Texture2DMSArray.ArraySize = arraysize;
					}
					else
					{
						depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
						depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
						depthStencilViewDesc.Texture2DArray.ArraySize = arraysize;
						depthStencilViewDesc.Texture2DArray.MipSlice = 0;
					}
				}
				else
				{
					if (multisample)
					{
						depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
					}
					else
					{
						depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
						depthStencilViewDesc.Texture2D.MipSlice = 0;
					}
				}
				hr = device->CreateDepthStencilView((ID3D11Resource*)pTexture->resource,&depthStencilViewDesc, (ID3D11DepthStencilView**)&pTexture->DSV);
			}
		}
		assert(SUCCEEDED(hr) && "DepthStencilView Creation failed!");
	}

	return hr;
}

HRESULT RenderDevice_DX11::CreateInputLayout(const VertexLayoutDesc *pInputElementDescs, UINT NumElements, GraphicBlob* blob, VertexLayout *pInputLayout)
{
	
	pInputLayout->Register(this);

	pInputLayout->desc.reserve((size_t)NumElements);

	D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[NumElements];
	for (UINT i = 0; i < NumElements; ++i)
	{
		desc[i].SemanticName = pInputElementDescs[i].SemanticName;
		desc[i].SemanticIndex = pInputElementDescs[i].SemanticIndex;
		desc[i].Format = ConvertFormat(pInputElementDescs[i].Format);
		desc[i].InputSlot = pInputElementDescs[i].InputSlot;
		desc[i].AlignedByteOffset = pInputElementDescs[i].AlignedByteOffset;
		if (desc[i].AlignedByteOffset == APPEND_ALIGNED_ELEMENT)
			desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc[i].InputSlotClass = ConvertInputClassification(pInputElementDescs[i].InputSlotClass);
		desc[i].InstanceDataStepRate = pInputElementDescs[i].InstanceDataStepRate;

		pInputLayout->desc.push_back(pInputElementDescs[i]);
	}

	HRESULT hr = device->CreateInputLayout(desc, NumElements, ((ID3D10Blob*)(blob->resourceDX))->GetBufferPointer(), ((ID3D10Blob*)(blob->resourceDX))->GetBufferSize(), (ID3D11InputLayout**)&pInputLayout->resource);

	SAFE_DELETE_ARRAY(desc);

	return hr;
}

HRESULT RenderDevice_DX11::CreateVertexShader(WCHAR* filename, GraphicBlob* blob, VertexShader* pVertexShader)
{

	pVertexShader->Register(this);
	ID3D10Blob* ERRORMESSAGE;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_DEBUG|D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resourceDX, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		ASSERT("vs create fail");
	}
	return device->CreateVertexShader(((ID3D10Blob*)(blob->resourceDX))->GetBufferPointer(), ((ID3D10Blob*)(blob->resourceDX))->GetBufferSize(), nullptr, (ID3D11VertexShader**)&pVertexShader->resource);
}

HRESULT RenderDevice_DX11::CreatePixelShader(WCHAR* filename, GraphicBlob* blob, PixelShader* pPixelShader)
{
	pPixelShader->Register(this);
	ID3D10Blob* ERRORMESSAGE;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resourceDX, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		ASSERT("ps create fail");
	}
	return device->CreatePixelShader(((ID3D10Blob*)(blob->resourceDX))->GetBufferPointer(), ((ID3D10Blob*)(blob->resourceDX))->GetBufferSize(), nullptr, (ID3D11PixelShader**)&pPixelShader->resource);
}

HRESULT RenderDevice_DX11::CreateGemotryShader(WCHAR* filename, GraphicBlob* blob, GeometryShader* pGeometryShader)
{
	pGeometryShader->Register(this);
	ID3D10Blob* ERRORMESSAGE = NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "gs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resourceDX, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		ASSERT("gs create fail");
	}
	return device->CreateGeometryShader(((ID3D10Blob*)(blob->resourceDX))->GetBufferPointer(), ((ID3D10Blob*)(blob->resourceDX))->GetBufferSize(), nullptr, (ID3D11GeometryShader**)&pGeometryShader->resource);
}

HRESULT RenderDevice_DX11::CreateHullShader(WCHAR* filename, GraphicBlob* blob, HullShader* pHullShader)
{
	pHullShader->Register(this);
	ID3D10Blob* ERRORMESSAGE = NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "hs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resourceDX, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		ASSERT("hs create fail");
	}
	return device->CreateHullShader(((ID3D10Blob*)(blob->resourceDX))->GetBufferPointer(), ((ID3D10Blob*)(blob->resourceDX))->GetBufferSize(), nullptr, (ID3D11HullShader**)&pHullShader->resource);
}

HRESULT RenderDevice_DX11::CreateDomainShader(WCHAR* filename, GraphicBlob* blob, DomainShader* pDomainShader)
{
	pDomainShader->Register(this);
	ID3D10Blob* ERRORMESSAGE = NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "ds_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resourceDX, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		ASSERT("ds create fail");
	}
	return device->CreateDomainShader(((ID3D10Blob*)(blob->resourceDX))->GetBufferPointer(), ((ID3D10Blob*)(blob->resourceDX))->GetBufferSize(), nullptr, (ID3D11DomainShader**)&pDomainShader->resource);
}
HRESULT RenderDevice_DX11::CreateComputerShader(WCHAR* filename, GraphicBlob* blob, ComputerShader* pComputerShader)
{
	pComputerShader->Register(this);
	ID3D10Blob* ERRORMESSAGE = NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resourceDX, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		ASSERT("cs create fail");
	}
	return device->CreateComputeShader(((ID3D10Blob*)(blob->resourceDX))->GetBufferPointer(), ((ID3D10Blob*)(blob->resourceDX))->GetBufferSize(), nullptr, (ID3D11ComputeShader**)&pComputerShader->resource);
}

HRESULT RenderDevice_DX11::CreateBlendState(const BlendStateDesc *pBlendStateDesc, BlendState *pBlendState)
{
	pBlendState->Register(this);

	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = pBlendStateDesc->AlphaToConvrageEnable;
	desc.IndependentBlendEnable = pBlendStateDesc->IndependentBlendEnable;
	for (int i=0;i<8;++i)
	{
		desc.RenderTarget[i].BlendEnable = pBlendStateDesc->RenderTarget[i].BlendEnable;
		desc.RenderTarget[i].SrcBlend = ConvertBlend(pBlendStateDesc->RenderTarget[i].SrcBlend);
		desc.RenderTarget[i].DestBlend = ConvertBlend(pBlendStateDesc->RenderTarget[i].DestBlend);
		desc.RenderTarget[i].BlendOp = ConvertBlendOp(pBlendStateDesc->RenderTarget[i].BlendOp);
		desc.RenderTarget[i].SrcBlendAlpha = ConvertBlend(pBlendStateDesc->RenderTarget[i].SrcBlendAlpha);
		desc.RenderTarget[i].DestBlendAlpha = ConvertBlend(pBlendStateDesc->RenderTarget[i].DestBlendAlpha);
		desc.RenderTarget[i].BlendOpAlpha = ConvertBlendOp(pBlendStateDesc->RenderTarget[i].BlendOpAlpha);
		desc.RenderTarget[i].RenderTargetWriteMask = ParseColorWriteMask(pBlendStateDesc->RenderTarget[i].RenderTargetWriteMask);

	}

	pBlendState->desc = *pBlendStateDesc;
	return device->CreateBlendState(&desc, (ID3D11BlendState**)&pBlendState->resource);

}

HRESULT RenderDevice_DX11::CreateDepthStencilState(const DepthStencilStateDesc* pDepthStencilStateDesc, DepthStencilState* pDepthStencilState)
{
	pDepthStencilState->Register(this);

	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = pDepthStencilStateDesc->DepthEnable;
	desc.DepthWriteMask = ConvertDepthWriteMask(pDepthStencilStateDesc->DepthWriteMask);
	desc.DepthFunc = ConvertComparisonFunc(pDepthStencilStateDesc->DepthFunc);
	desc.StencilEnable = pDepthStencilStateDesc->StencilEnable;
	desc.StencilReadMask = pDepthStencilStateDesc->StencilReadMask;
	desc.StencilWriteMask = pDepthStencilStateDesc->StencilWriteMask;
	desc.FrontFace.StencilDepthFailOp = ConvertStencilOp(pDepthStencilStateDesc->FrontFace.StencilDepthFailOp);
	desc.FrontFace.StencilFailOp = ConvertStencilOp(pDepthStencilStateDesc->FrontFace.StencilFailOp);
	desc.FrontFace.StencilFunc = ConvertComparisonFunc(pDepthStencilStateDesc->FrontFace.StencilFunc);
	desc.FrontFace.StencilPassOp = ConvertStencilOp(pDepthStencilStateDesc->FrontFace.StencilPassOp);
	desc.BackFace.StencilDepthFailOp = ConvertStencilOp(pDepthStencilStateDesc->BackFace.StencilDepthFailOp);
	desc.BackFace.StencilFunc = ConvertComparisonFunc(pDepthStencilStateDesc->BackFace.StencilFunc);
	desc.BackFace.StencilPassOp = ConvertStencilOp(pDepthStencilStateDesc->BackFace.StencilPassOp);

	pDepthStencilState->desc = *pDepthStencilStateDesc;
	return device->CreateDepthStencilState(&desc, (ID3D11DepthStencilState**)&pDepthStencilState->resource);
}

HRESULT RenderDevice_DX11::CreateRasterizerState(const RasterizerStateDesc *pRasterizerStateDesc, RasterizerState *pRasterizerState)
{
	pRasterizerState->Register(this);

	pRasterizerState->desc = *pRasterizerStateDesc;

	D3D11_RASTERIZER_DESC desc;
	desc.FillMode = ConvertFillMode(pRasterizerStateDesc->FillMode);
	desc.CullMode = ConvertCullMode(pRasterizerStateDesc->CullMode);
	desc.FrontCounterClockwise = pRasterizerStateDesc->FrontCounterClockWise;
	desc.DepthBias = pRasterizerStateDesc->DepthBias;
	desc.DepthBiasClamp = pRasterizerStateDesc->DepthBiasClamp;
	desc.SlopeScaledDepthBias = pRasterizerStateDesc->SlopeScaledDepthBias;
	desc.DepthClipEnable = pRasterizerStateDesc->DepthCilpEnable;
	desc.ScissorEnable = true;
	desc.MultisampleEnable = pRasterizerStateDesc->MultisampleEnable;
	desc.AntialiasedLineEnable = pRasterizerStateDesc->AntialiasedLineEnable;

	if (CONSERVATIVE_RASTERIZATION && pRasterizerStateDesc->ConservativeRasterizationEnable==TRUE)
	{
		ID3D11Device3* device3 = nullptr;
		if (SUCCEEDED(device->QueryInterface(__uuidof(ID3D11Device),(void**)&device3)))
		{
			D3D11_RASTERIZER_DESC2 desc2;
			desc2.FillMode = desc.FillMode;
			desc2.CullMode = desc.CullMode;
			desc2.FrontCounterClockwise = desc.FrontCounterClockwise;
			desc2.DepthBias = desc.DepthBias;
			desc2.DepthBiasClamp = desc.DepthBiasClamp;
			desc2.SlopeScaledDepthBias = desc.SlopeScaledDepthBias;
			desc2.DepthClipEnable = desc.DepthClipEnable;
			desc2.ScissorEnable = desc.ScissorEnable;
			desc2.MultisampleEnable = desc.MultisampleEnable;
			desc2.AntialiasedLineEnable = desc.AntialiasedLineEnable;
			desc2.ForcedSampleCount = (RASTERIZER_ORDERED_VIEWS ? pRasterizerStateDesc->ForceSampleCount : 0);

			pRasterizerState->desc = *pRasterizerStateDesc;

			ID3D11RasterizerState2* rasterizer2 = nullptr;
			HRESULT hr = device3->CreateRasterizerState2(&desc2, &rasterizer2);
			pRasterizerState->resource = (CPUHandle)rasterizer2;
			SAFE_RELEASE(device3);
			return hr;
		}
	}

	else if (RASTERIZER_ORDERED_VIEWS && pRasterizerStateDesc->ForceSampleCount>0)
	{
		ID3D11Device1* device1 = nullptr;
		if (SUCCEEDED(device->QueryInterface(__uuidof(ID3D11Device1),(void**)&device1)))
		{
			D3D11_RASTERIZER_DESC1 desc1;
			desc1.FillMode = desc.FillMode;
			desc1.CullMode = desc.CullMode;
			desc1.FrontCounterClockwise = desc.FrontCounterClockwise;
			desc1.DepthBias = desc.DepthBias;
			desc1.SlopeScaledDepthBias = desc.SlopeScaledDepthBias;
			desc1.DepthClipEnable = desc.DepthClipEnable;
			desc1.ScissorEnable = desc.ScissorEnable;
			desc1.MultisampleEnable = desc.MultisampleEnable;
			desc1.AntialiasedLineEnable = desc.AntialiasedLineEnable;
			desc1.ForcedSampleCount = pRasterizerStateDesc->ForceSampleCount;

			pRasterizerState->desc = *pRasterizerStateDesc;

			ID3D11RasterizerState1* rasterizer1 = nullptr;
			HRESULT hr = device1->CreateRasterizerState1(&desc1, &rasterizer1);
			pRasterizerState->resource = (CPUHandle)rasterizer1;
			SAFE_RELEASE(device1);
			return hr;
		}
	}
	return device->CreateRasterizerState(&desc, (ID3D11RasterizerState**)&pRasterizerState->resource);
}

HRESULT RenderDevice_DX11::CreateSamplerState(const SamplerDesc *pSamplerDesc, Sampler *pSamplerState)
{
	pSamplerState->Register(this);

	D3D11_SAMPLER_DESC desc;
	desc.Filter = ConvertFilter(pSamplerDesc->Filter);
	desc.AddressU = ConvertTextureAddressMode(pSamplerDesc->AddressU);
	desc.AddressV = ConvertTextureAddressMode(pSamplerDesc->AddressV);
	desc.AddressW = ConvertTextureAddressMode(pSamplerDesc->AddressW);
	desc.MipLODBias = pSamplerDesc->MipLODBias;
	desc.MaxAnisotropy = pSamplerDesc->MaxAnisotropy;
	desc.ComparisonFunc = ConvertComparisonFunc(pSamplerDesc->ComparsionFunc);
	desc.BorderColor[0] = pSamplerDesc->BorderColor[0];
	desc.BorderColor[1] = pSamplerDesc->BorderColor[1]; 
	desc.BorderColor[2] = pSamplerDesc->BorderColor[2];
	desc.BorderColor[3] = pSamplerDesc->BorderColor[3];
	desc.MinLOD = pSamplerDesc->MinLOD;
	desc.MaxLOD = pSamplerDesc->MaxLOD;

	pSamplerState->desc = *pSamplerDesc;
	return device->CreateSamplerState(&desc, (ID3D11SamplerState**)&pSamplerState->resource);
}

HRESULT RenderDevice_DX11::CreateQuery(const GPUQueryDesc *pDesc, GPUQuery *pQuery)
{
	pQuery->Register(this);

	HRESULT hr = E_FAIL;

	pQuery->desc = *pDesc;
	pQuery->async_frameshift = pQuery->desc.async_Latnce;

	D3D11_QUERY_DESC desc;
	desc.MiscFlags = 0;
	desc.Query = D3D11_QUERY_OCCLUSION_PREDICATE;
	if (pDesc->Type==GPU_QUERY_TYPE_OCCLUSION)
	{
		desc.Query = D3D11_QUERY_OCCLUSION;
	}
	else if (pDesc->Type== GPU_QUERY_TYPE_TIMESTAMP)
	{
		desc.Query = D3D11_QUERY_TIMESTAMP;
	}
	else if (pDesc->Type == GPU_QUERY_TYPE_TIMESTAMP_DISJOINT)
	{
		desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	}

	if (pQuery->desc.async_Latnce > 0)
	{
		pQuery->resource.resize(pQuery->desc.async_Latnce + 1);
		pQuery->active.resize(pQuery->desc.async_Latnce + 1);
		for (size_t i=0;i<pQuery->resource.size();++i) 
		{
			hr = device->CreateQuery(&desc, (ID3D11Query**)&pQuery->resource[i]);
			assert(SUCCEEDED(hr) && "GPUQuery creation failed!");
		}
	}
	else
	{
		pQuery->resource.resize(1);
		pQuery->active.resize(1);
		hr = device->CreateQuery(&desc, (ID3D11Query**)&pQuery->resource[0]);
		assert(SUCCEEDED(hr) && "GPUQuery creation failed!");
	}
	return hr;

}

HRESULT RenderDevice_DX11::CreateGraphicPSO(const GraphicsShaderDesc* pDesc, GraphicPSO* pso)
{
	pso->Register(this);

	pso->desc = *pDesc;

	return S_OK;
}

HRESULT RenderDevice_DX11::CreateComputerPSO(const ComputerPSODesc* pDesc, ComputerPSO* pso)
{
	pso->Register(this);

	pso->desc = *pDesc;

	return S_OK;
}


void RenderDevice_DX11::DestroyResource(GPUResource* pResource)
{
	if (pResource->resource != NUll_Handle)
	{
		((ID3D11Resource*)pResource->resource)->Release();
	}

	if (pResource->SRV != NUll_Handle)
	{
		((ID3D11ShaderResourceView*)pResource->SRV)->Release();
	}
	for (auto& x : pResource->additionalSRVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D11ShaderResourceView*)x)->Release();
		}
	}

	if (pResource->UAV != NUll_Handle)
	{
		((ID3D11UnorderedAccessView*)pResource->UAV)->Release();
	}
	for (auto& x : pResource->additionalUAVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D11UnorderedAccessView*)x)->Release();
		}
	}
}

void RenderDevice_DX11::DestroyBuffer(GPUBuffer* pBuffer)
{
	
}

void RenderDevice_DX11::DestroyTexture1D(Texture1D* pTexture1D)
{
	if (pTexture1D->RTV != NUll_Handle)
	{
		((ID3D11RenderTargetView*)pTexture1D->RTV)->Release();
	}
	for (auto& x : pTexture1D->additionalRTVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D11RenderTargetView*)x)->Release();
		}
	}
}

void RenderDevice_DX11::DestroyTexture2D(Texture2D* pTexture2D)
{
	if (pTexture2D->RTV != NUll_Handle)
	{
		((ID3D11RenderTargetView*)pTexture2D->RTV)->Release();
	}
	for (auto& x : pTexture2D->additionalRTVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D11RenderTargetView*)x)->Release();
		}
	}

	if (pTexture2D->DSV != NUll_Handle)
	{
		((ID3D11DepthStencilView*)pTexture2D->DSV)->Release();
	}
	for (auto& x : pTexture2D->additionalDSVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D11DepthStencilView*)x)->Release();
		}
	}

}

void RenderDevice_DX11::DestroyTexture3D(Texture3D* pTexture3D)
{
	if (pTexture3D->RTV != NUll_Handle)
	{
		((ID3D11RenderTargetView*)pTexture3D->RTV)->Release();
	}
	for (auto& x : pTexture3D->additionalRTVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D11RenderTargetView*)x)->Release();
		}
	}
}

void RenderDevice_DX11::DestroyInputLayout(VertexLayout *pInputLayout)
{
	if (pInputLayout->resource != NUll_Handle)
	{
		((ID3D11InputLayout*)pInputLayout->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyVertexShader(VertexShader* pVertexShader)
{
	if (pVertexShader->resource != NUll_Handle)
	{
		((ID3D11VertexShader*)pVertexShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyPixelShader(PixelShader* pPixelShader)
{

	if (pPixelShader->resource != NUll_Handle)
	{
		((ID3D11PixelShader*)pPixelShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyGeometryShader(GeometryShader* pGeometryShader)
{
	if (pGeometryShader->resource != NUll_Handle)
	{
		((ID3D11GeometryShader*)pGeometryShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyHullShader(HullShader* pHullShader)
{
	if (pHullShader->resource != NUll_Handle)
	{
		((ID3D11HullShader*)pHullShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyDomainShader(DomainShader* pDomainShader)
{
	if (pDomainShader->resource != NUll_Handle)
	{
		((ID3D11DomainShader*)pDomainShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyComputerShader(ComputerShader* pComputerShader)
{
	if (pComputerShader->resource != NUll_Handle)
	{
		((ID3D11ComputeShader*)pComputerShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyBlendState(BlendState* pBlendState)
{
	if (pBlendState->resource != NUll_Handle)
	{
		((ID3D11BlendState*)pBlendState->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyDepthStencilState(DepthStencilState* pDepthStencilState)
{
	if (pDepthStencilState->resource != NUll_Handle)
	{
		((ID3D11DepthStencilState*)pDepthStencilState->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyRasterizerState(RasterizerState* pRasterizerstate)
{
	if (pRasterizerstate->resource != NUll_Handle)
	{
		((ID3D11RasterizerState*)pRasterizerstate->resource)->Release();
	}
}

void RenderDevice_DX11::DestroySamplerState(Sampler* pSamplerState)
{
	if (pSamplerState->resource != NUll_Handle)
	{
		((ID3D11SamplerState*)pSamplerState->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyQuery(GPUQuery *pQuey)
{
	for (auto& x : pQuey->resource)
	{
		if (x != NUll_Handle)
		{
			((ID3D11Query*)x)->Release();
		}
	}
}

void RenderDevice_DX11::DestroyGraphicsPSO(GraphicPSO* pso)
{

}

void RenderDevice_DX11::DestroyComputerPSO(ComputerPSO* pso)
{

}

void RenderDevice_DX11::SetName(GPUResource* pResource, const std::string&name)
{
	((ID3D11Resource*)pResource->resource)->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
}

void RenderDevice_DX11::PresentBegin()
{
	const float color[4] = { 1.f, 1.0f, 1.000000000f, 1.000000000f };
	ImmediatedeviceContext->ClearRenderTargetView(renderTargetView, color);
	ImmediatedeviceContext->ClearDepthStencilView(DepthStecilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void RenderDevice_DX11::PresentEnd()
{
	DXGI_PRESENT_PARAMETERS parameters = { 0 };
	swapChain->Present1(VSYNC, 0,&parameters);

/*
	deviceContexts[GRAPHICTHREAD_IMMERIATE]->OMSetRenderTargets(0, nullptr, nullptr);

	deviceContexts[GRAPHICTHREAD_IMMERIATE]->ClearState();
	BindGraphicsPSO(nullptr);
	BindComputerPSO(nullptr);

	D3D11_RECT pRects[8];
	for (UINT i=0;i<8;++i)
	{
		pRects[i].bottom = INT32_MAX;
		pRects[i].left = INT32_MIN;
		pRects[i].right = INT32_MAX;
		pRects[i].top = INT32_MIN;
	}
	deviceContexts[GRAPHICTHREAD_IMMERIATE]->RSSetScissorRects(0, pRects);*/

/*
	memset(prev_vs, 0, sizeof(prev_vs));
	memset(prev_ps, 0, sizeof(prev_ps));
	memset(prev_hs, 0, sizeof(prev_hs));
	memset(prev_ds, 0, sizeof(prev_ds));
	memset(prev_gs, 0, sizeof(prev_gs));
	memset(prev_blendfactor, 0, sizeof(prev_blendfactor));
	memset(prev_samplemask, 0, sizeof(prev_samplemask));
	memset(prev_bs, 0, sizeof(prev_bs));
	memset(prev_rs, 0, sizeof(prev_rs));
	memset(prev_stencilRef, 0, sizeof(prev_stencilRef));
	memset(prev_dss, 0, sizeof(prev_dss));
	memset(prev_vl, 0, sizeof(prev_vl));
	memset(prev_pt, 0, sizeof(prev_pt));

	memset(raster_uavs, 0, sizeof(raster_uavs));
	memset(raster_uavs_slot, 0, sizeof(raster_uavs_slot));
	memset(raster_uavs_count, 0, sizeof(raster_uavs_count));*/

	FRAMECOUNT++;


	RESOLUTIONCHANGED = false;
}

void RenderDevice_DX11::ExcuteDeferredContexts()
{
		for (int i=0;i<8;i++)
		{
			if (commandlists[i]!=nullptr)
			{
				ImmediatedeviceContext->ExecuteCommandList(commandlists[i], false);
			}
			
		}
		
}

void RenderDevice_DX11::FinishComanlist()
{

	deviceContexts[ThreadID]->FinishCommandList(false, &commandlists[ThreadID]);
	
}

void RenderDevice_DX11::validate_raster_uavs(uint64_t ThreadID)
{
	// This is a helper function to defer the graphics stage UAV bindings to OMSetRenderTargetsAndUnorderedAccessViews (if there was an update)
	//	It is intended to be called before graphics stage executions (Draw)
	//	It is also explicitly maintained in BindRenderTargets function, because in that case, we will also bind some render targets in the same call
/*
	if (raster_uavs_count[ThreadID]>0)
	{
		const UINT count = raster_uavs_count[ThreadID];
		const UINT slot = raster_uavs_slot[ThreadID];

		deviceContexts[ThreadID]->OMSetRenderTargetsAndUnorderedAccessViews(0, nullptr, nullptr, slot, count, &raster_uavs[ThreadID][ThreadID],nullptr);

		raster_uavs_count[ThreadID] = 0;
		raster_uavs_slot[ThreadID] = 8;
	}*/
}


void RenderDevice_DX11::CreateRenderTargetAndDepthStencil()
{
	SAFE_RELEASE(backbuffer);
	SAFE_RELEASE(renderTargetView);

	HRESULT hr;

	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			_T("Get SwapChain failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
	}

	hr=device->CreateRenderTargetView(backbuffer, NULL, &renderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			_T("Create RenderTarget failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
	}
	SAFE_RELEASE(backbuffer);
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = SCREENWIDTH;
	depthStencilDesc.Height = SCREENHEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	Basedevice->CreateTexture2D(&depthStencilDesc, NULL, &depthbuffer);
	Basedevice->CreateDepthStencilView(depthbuffer, NULL, &DepthStecilView);
}

void RenderDevice_DX11::BindScissorRects(UINT numRects, const Rect* rect)
{
	assert(rect!=nullptr);
	assert(numRects <= 8);
	D3D11_RECT pRects[8];
	for (UINT i=0;i<numRects;++i)
	{
		pRects[i].bottom = rect[i].bottom;
		pRects[i].left = rect[i].left;
		pRects[i].right = rect[i].right;
		pRects[i].top = rect[i].top;
	}
	deviceContexts[ThreadID]->RSSetScissorRects(numRects, pRects);
}

void RenderDevice_DX11::BindViewports(UINT NumViewports, const ViewPort* pViewports)
{
	assert(NumViewports <= 6);
	D3D11_VIEWPORT d3dViewPorts[6];
	for (UINT i=0;i<NumViewports;++i)
	{
		d3dViewPorts[i].TopLeftX = pViewports[i].TopLeftX;
		d3dViewPorts[i].TopLeftY = pViewports[i].TopLeftY;
		d3dViewPorts[i].Width = pViewports[i].Width;
		d3dViewPorts[i].Height = pViewports[i].Height;
		d3dViewPorts[i].MinDepth = pViewports[i].MinDepth;
		d3dViewPorts[i].MaxDepth = pViewports[i].MaxDepth;
	}

	deviceContexts[ThreadID]->RSSetViewports(NumViewports, d3dViewPorts);
}

void RenderDevice_DX11::BindRenderTargets(UINT NumViews, Texture2D* const *ppRenderTargets, Texture2D* depthStencilTexture, int arrayIndex/* = -1*/)
{
	ID3D11RenderTargetView* rendertargetViews[8];
	ZeroMemory(rendertargetViews, sizeof(rendertargetViews));
	for (UINT i = 0; i < min(NumViews, 8); ++i)
	{
		if (arrayIndex < 0 || ppRenderTargets[i]->additionalRTVs.empty())
		{
			rendertargetViews[i] = (ID3D11RenderTargetView*)ppRenderTargets[i]->RTV;
		}
		else
		{
			assert(ppRenderTargets[i]->additionalRTVs.size() > static_cast<size_t>(arrayIndex) && "Invaild");
			rendertargetViews[i] = (ID3D11RenderTargetView*)ppRenderTargets[i]->additionalRTVs[arrayIndex];
		}
	}

	ID3D11DepthStencilView* depthStencilView = nullptr;
	if (depthStencilView!=nullptr)
	{
		if (arrayIndex<0||depthStencilTexture->additionalDSVs.empty())
		{
			depthStencilView = (ID3D11DepthStencilView*)depthStencilTexture->DSV;
		}
		else
		{
			assert(depthStencilTexture->additionalDSVs.size() > static_cast<size_t>(arrayIndex) && "Ivalide");
			depthStencilView = (ID3D11DepthStencilView*)depthStencilTexture->additionalDSVs[arrayIndex];
		}
	}

	if (raster_uavs_count[GRAPHICTHREAD_IMMERIATE]>0)
	{
		const UINT count = raster_uavs_count[GRAPHICTHREAD_IMMERIATE];
		const UINT slot = raster_uavs_slot[GRAPHICTHREAD_IMMERIATE];

		deviceContexts[GRAPHICTHREAD_IMMERIATE]->OMSetRenderTargetsAndUnorderedAccessViews(NumViews, rendertargetViews, depthStencilView, slot, count, &raster_uavs[ThreadID][slot], nullptr);

		raster_uavs_count[GRAPHICTHREAD_IMMERIATE] = 0;
		raster_uavs_slot[GRAPHICTHREAD_IMMERIATE] = 8;
	}
	else
	{
		deviceContexts[ThreadID]->OMSetRenderTargets(NumViews, rendertargetViews, depthStencilView);
	}

}



void RenderDevice_DX11::ClearRenderTarget(Texture* pTexture, const FLOAT ColorRGBA[4],int arrayIndex /*= -1*/)
{
	if (arrayIndex<0)
	{
		deviceContexts[ThreadID]->ClearRenderTargetView((ID3D11RenderTargetView*)pTexture->RTV, ColorRGBA);
	}
	else
	{
		assert(pTexture->additionalRTVs.size() > static_cast<size_t>(arrayIndex) && "Invalid");
		deviceContexts[ThreadID]->ClearRenderTargetView((ID3D11RenderTargetView*)pTexture->additionalRTVs[arrayIndex], ColorRGBA);
	}
}

void RenderDevice_DX11::ClearDepthStencil(Texture2D* pTexture, UINT ClearFlags, FLOAT Depth, UINT8 Stencil, int arrayIndex /*= -1*/)
{
	UINT flag = 0;
	if (ClearFlags&CLEAR_DEPTH)
	{
		flag |= D3D11_CLEAR_DEPTH;
	}
	if (ClearFlags&CLEAR_STENCIL)
	{
		flag |= D3D11_CLEAR_STENCIL;
	}

	if (arrayIndex<0)
	{
		deviceContexts[ThreadID]->ClearDepthStencilView((ID3D11DepthStencilView*)pTexture->DSV, flag, Depth, Stencil);
	}
	else
	{
		assert(pTexture->additionalDSVs.size() > static_cast<size_t>(arrayIndex) && "Invalid");
		deviceContexts[ThreadID]->ClearDepthStencilView((ID3D11DepthStencilView*)pTexture->additionalDSVs[arrayIndex], flag, Depth, Stencil);
	}
}

void RenderDevice_DX11::BindResource(SHADERSTAGE stage, GPUResource* resource, int slot, int arrayIndex /*= -1*/)
{
	if (resource != nullptr)
	{
		ID3D11ShaderResourceView* SRV;

		if (arrayIndex < 0)
		{
			SRV = (ID3D11ShaderResourceView*)resource->SRV;
		}
		else
		{
			assert(resource->additionalSRVs.size() > static_cast<size_t>(arrayIndex) && "Invalid arrayIndex!");
			SRV = (ID3D11ShaderResourceView*)resource->additionalSRVs[arrayIndex];
		}

		switch (stage)
		{
		case PRE::VS_STAGE:
			deviceContexts[ThreadID]->VSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::HS_STAGE:
			deviceContexts[ThreadID]->HSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::DS_STAGE:
			deviceContexts[ThreadID]->DSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::GS_STAGE:
			deviceContexts[ThreadID]->GSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::PS_STAGE:
			deviceContexts[ThreadID]->PSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::CS_STAGE:
			deviceContexts[ThreadID]->CSSetShaderResources(slot, 1, &SRV);
			break;
		default:
			assert(0);
			break;
		}
	}
}

void RenderDevice_DX11::BindResources(SHADERSTAGE stage, GPUResource *const* resources, int slot, int count)
{
	assert(count <= 8);
	ID3D11ShaderResourceView* srvs[8];
	for (int i = 0; i < count; ++i)
	{
		srvs[i] = resources[i] != nullptr ? (ID3D11ShaderResourceView*)resources[i]->SRV : nullptr;
	}
	

	switch (stage)
	{
	case PRE::VS_STAGE:
		deviceContexts[ThreadID]->VSSetShaderResources(slot, count, srvs);
		break;
	case PRE::HS_STAGE:
		deviceContexts[ThreadID]->HSSetShaderResources(slot, count, srvs);
		break;
	case PRE::DS_STAGE:
		deviceContexts[ThreadID]->DSSetShaderResources(slot, count, srvs);
		break;
	case PRE::GS_STAGE:
		deviceContexts[ThreadID]->GSSetShaderResources(slot, count, srvs);
		break;
	case PRE::PS_STAGE:
		deviceContexts[ThreadID]->PSSetShaderResources(slot, count, srvs);
		break;
	case PRE::CS_STAGE:
		deviceContexts[ThreadID]->CSSetShaderResources(slot, count, srvs);
		break;
	default:
		assert(0);
		break;
	}
}

void RenderDevice_DX11::BindUAV(SHADERSTAGE stage, GPUResource * resources, int slot,int arrayIndex /*= -1*/)
{
	if (resources != nullptr)
	{
		ID3D11UnorderedAccessView* UAV = (ID3D11UnorderedAccessView*)resources->UAV;

		if (arrayIndex < 0)
		{
			UAV = (ID3D11UnorderedAccessView*)resources->UAV;
		}
		else
		{
			assert(resources->additionalUAVs.size() > static_cast<size_t>(arrayIndex) && "Invalid arrayIndex!");
			UAV = (ID3D11UnorderedAccessView*)resources->additionalUAVs[arrayIndex];
		}

		if (stage == CS_STAGE)
		{
			deviceContexts[ThreadID]->CSSetUnorderedAccessViews(slot, 1, &UAV, nullptr);
		}
		else
		{
			raster_uavs[ThreadID][slot] = (ID3D11UnorderedAccessView*)resources->UAV;
			raster_uavs_slot[ThreadID] = min(raster_uavs_slot[ThreadID], slot);
			raster_uavs_count[ThreadID] = max(raster_uavs_count[ThreadID], 1);
		}
	}
}

void RenderDevice_DX11::BindUAVS(SHADERSTAGE stage, GPUResource* const* resource, int slot, int count)
{
	assert(slot + count <= 8);
	ID3D11UnorderedAccessView* uavs[8];
	for (int i=0;i<count;++i)
	{
		uavs[i] = resource[i] != nullptr ? (ID3D11UnorderedAccessView*)resource[i]->UAV : nullptr;

		if (stage!=CS_STAGE)
		{
			raster_uavs[ThreadID][slot + i] = uavs[i];
		}
	}

	if (stage==CS_STAGE)
	{
		deviceContexts[ThreadID]->CSSetUnorderedAccessViews(static_cast<UINT>(slot), static_cast<UINT>(count), uavs, nullptr);
	}
	else
	{
		raster_uavs_slot[ThreadID] = min(raster_uavs_slot[ThreadID], slot);
		raster_uavs_count[ThreadID] = max(raster_uavs_count[ThreadID], count);
	}
}

void RenderDevice_DX11::UnbindResources(int slot, int num)
{
	assert(num <= ARRAYSIZE(nullBlob) && "Extend nullBlob to support more resource unbinding");
	deviceContexts[ThreadID]->PSSetShaderResources(slot, num, (ID3D11ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->VSSetShaderResources(slot, num, (ID3D11ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->GSSetShaderResources(slot, num, (ID3D11ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->HSSetShaderResources(slot, num, (ID3D11ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->DSSetShaderResources(slot, num, (ID3D11ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->CSSetShaderResources(slot, num, (ID3D11ShaderResourceView**)nullBlob);
}

void RenderDevice_DX11::UnbindUAVs(int slot, int num)
{
	assert(num <= ARRAYSIZE(nullBlob) && "Extend nullBlob to support more resource unbinding");
	deviceContexts[ThreadID]->CSSetUnorderedAccessViews(slot, num, (ID3D11UnorderedAccessView**)nullBlob, 0);

	raster_uavs_count[ThreadID] = 0;
	raster_uavs_slot[ThreadID] = 8;
}

void RenderDevice_DX11::BindSampler(SHADERSTAGE stage, Sampler* sampler, ShaderReflection* sf)
{

	ShaderReflectionDX* sfdx = (ShaderReflectionDX*)sf;
	ID3D11SamplerState* SAMPLER = (ID3D11SamplerState*)sampler->resource;


	for (ShaderInputBindDescDX* psfdx:sfdx->ResourceBindings)
	{
		if (psfdx->Type==D3D_SIT_SAMPLER)
		{
			switch (stage)
			{
			case PRE::VS_STAGE:
				deviceContexts[ThreadID]->VSSetSamplers(psfdx->BindPoint, 1, &SAMPLER);
				break;
			case PRE::HS_STAGE:
				deviceContexts[ThreadID]->HSSetSamplers(psfdx->BindPoint, 1, &SAMPLER);
				break;
			case PRE::DS_STAGE:
				deviceContexts[ThreadID]->DSSetSamplers(psfdx->BindPoint, 1, &SAMPLER);
				break;
			case PRE::GS_STAGE:
				deviceContexts[ThreadID]->GSSetSamplers(psfdx->BindPoint, 1, &SAMPLER);
				break;
			case PRE::PS_STAGE:
				deviceContexts[ThreadID]->PSSetSamplers(psfdx->BindPoint, 1, &SAMPLER);
				break;
			case PRE::CS_STAGE:
				deviceContexts[ThreadID]->CSSetSamplers(psfdx->BindPoint, 1, &SAMPLER);
				break;
			default:
				assert(0);
				break;
			}
		}
	}


}

void RenderDevice_DX11::BindConstantBuffer(SHADERSTAGE stage, GPUBuffer* buffer, int slot, const UINT *pFirstConstant, const UINT *pNumberConstant)
{
	ID3D11Buffer* res = buffer ? (ID3D11Buffer*)buffer->resource : nullptr;
	switch (stage)
	{
	case PRE::VS_STAGE:
		deviceContexts[ThreadID]->VSSetConstantBuffers1(slot, 1, &res,pFirstConstant,pNumberConstant);
		break;
	case PRE::HS_STAGE:
		deviceContexts[ThreadID]->HSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::DS_STAGE:
		deviceContexts[ThreadID]->DSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::GS_STAGE:
		deviceContexts[ThreadID]->GSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::PS_STAGE:
		deviceContexts[ThreadID]->PSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::CS_STAGE:
		deviceContexts[ThreadID]->CSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	default:
		assert(0);
		break;
	}
		
}

void RenderDevice_DX11::BindVertexBuffers(GPUBuffer* const *vertexBuffers, int slot, int count, const UINT* strides, const UINT*offsets)
{
	assert(count < 8);
	ID3D11Buffer* res[8] = { 0 };
	for (int i=0;i<count;++i)
	{
		res[i] = vertexBuffers[i] != nullptr ? (ID3D11Buffer*)vertexBuffers[i]->resource : nullptr;
	}
	deviceContexts[ThreadID]->IASetVertexBuffers(static_cast<UINT>(slot), static_cast<UINT>(count),res,strides, (offsets != nullptr ? offsets : reinterpret_cast<const UINT*>(nullBlob)));
}

void RenderDevice_DX11::BindIndexBuffer(GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, UINT offset)
{
	ID3D11Buffer* res = indexBuffer != nullptr ? (ID3D11Buffer*)indexBuffer->resource : nullptr;
	deviceContexts[ThreadID]->IASetIndexBuffer(res, (format == INDEXBUFFER_FORMAT::INDEXBUFFER_16BIT ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT),offset);
}

void RenderDevice_DX11::BindStencilRef(UINT value)
{
	stencilRef[ThreadID] = value;
}

void RenderDevice_DX11::BindBlendFactor(XMFLOAT4 value)
{
	blendFactor[ThreadID] = value;
}

void RenderDevice_DX11::BindGraphicsPSO(GraphicPSO* pso)
{
	const GraphicsShaderDesc& desc = pso != nullptr ? pso->GetDesc() : GraphicsShaderDesc();

	ID3D11VertexShader* vs = desc.vs == nullptr ? nullptr : (ID3D11VertexShader*)desc.vs->resource;
	if (vs !=nullptr)
	{
		deviceContexts[ThreadID]->VSSetShader(vs, nullptr, 0);
	}
	ID3D11PixelShader* ps = desc.ps == nullptr ? nullptr : (ID3D11PixelShader*)desc.ps->resource;
	if (ps != nullptr)
	{
		deviceContexts[ThreadID]->PSSetShader(ps, nullptr, 0);
	}
	ID3D11HullShader* hs = desc.hs == nullptr ? nullptr : (ID3D11HullShader*)desc.hs->resource;
	if (hs != nullptr)
	{
		deviceContexts[ThreadID]->HSSetShader(hs, nullptr, 0);
	}
	ID3D11DomainShader* ds = desc.ds == nullptr ? nullptr : (ID3D11DomainShader*)desc.ds->resource;
	if (ds != nullptr)
	{
		deviceContexts[ThreadID]->DSSetShader(ds, nullptr, 0);
	}
	ID3D11GeometryShader* gs = desc.gs == nullptr ? nullptr : (ID3D11GeometryShader*)desc.gs->resource;
	if (gs != nullptr)
	{
		deviceContexts[ThreadID]->GSSetShader(gs, nullptr, 0);
	}

	ID3D11BlendState* bs = desc.bs == nullptr ? nullptr : (ID3D11BlendState*)desc.bs->resource;
	if (bs != nullptr )
	{
		const float fact[4] = { blendFactor[0].x, blendFactor[0].y, blendFactor[0].z, blendFactor[0].w };
		deviceContexts[ThreadID]->OMSetBlendState(bs, fact, desc.sampleMask);
	}

	ID3D11RasterizerState* rs = desc.rs == nullptr ? nullptr : (ID3D11RasterizerState*)desc.rs->resource;
	if (rs != nullptr)
	{
		deviceContexts[ThreadID]->RSSetState(rs);
	}

	ID3D11DepthStencilState* dss = desc.dss == nullptr ? nullptr : (ID3D11DepthStencilState*)desc.dss->resource;
	if (dss != nullptr)
	{
		deviceContexts[ThreadID]->OMSetDepthStencilState(dss, stencilRef[0]);
	}

	ID3D11InputLayout* il = desc.VL == nullptr ? nullptr : (ID3D11InputLayout*)desc.VL->resource;
	if (il != nullptr)
	{
		deviceContexts[ThreadID]->IASetInputLayout(il);
	}

	{
		D3D11_PRIMITIVE_TOPOLOGY d3dType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		switch (desc.PT)
		{
		case TRIANGELIST:
			d3dType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		case TRIANGESTRIP:
			d3dType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;
		case POINTLIST:
			d3dType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		case LINELIST:
			d3dType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case PATCHLIST:
			d3dType = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
			break;
		default:
			d3dType = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			break;
		};
		deviceContexts[ThreadID]->IASetPrimitiveTopology(d3dType);
	}
}

void RenderDevice_DX11::BindRasterizerState(RasterizerState RSstate)
{
	deviceContexts[ThreadID]->RSSetState((ID3D11RasterizerState*)RSstate.resource);
}

void RenderDevice_DX11::BindComputerPSO(ComputerPSO* pso)
{
	const ComputerPSODesc& desc = pso != nullptr ? pso->GetDesc() : ComputerPSODesc();

	static ID3D11ComputeShader*prev_cs[GRAPHICSTHREAD_COUNT] = {};
	ID3D11ComputeShader* cs = desc.cs == NUll_Handle ? NUll_Handle : (ID3D11ComputeShader*)desc.cs->resource;
	if (cs != prev_cs[ThreadID])
	{
		deviceContexts[ThreadID]->CSSetShader(cs, nullptr, 0);
		prev_cs[ThreadID] = cs;
	}
}

void RenderDevice_DX11::Draw(int vertexCount, UINT startVertexLociotn)
{
	validate_raster_uavs(ThreadID);

	deviceContexts[ThreadID]->Draw(vertexCount, startVertexLociotn);
}

void RenderDevice_DX11::DrawIndexed(int Indexcount, UINT startVertexLocation, UINT baseVertexLocation)
{
	validate_raster_uavs(0);

	deviceContexts[ThreadID]->DrawIndexed(Indexcount, startVertexLocation, baseVertexLocation);
}

void RenderDevice_DX11::DrawInstanced(int vertexCount, int instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	validate_raster_uavs(ThreadID);

	deviceContexts[ThreadID]->DrawInstanced(vertexCount, instanceCount, startVertexLocation, startInstanceLocation);
}

void RenderDevice_DX11::DrawIndexedInstanced(int indexCount, int instanceCount, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanced)
{
	validate_raster_uavs(ThreadID);

	deviceContexts[ThreadID]->DrawIndexedInstanced(indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanced);
}

void RenderDevice_DX11::DrawInstancedIndirect(GPUBuffer* args, UINT args_offset)
{
	validate_raster_uavs(ThreadID);

	deviceContexts[ThreadID]->DrawInstancedIndirect((ID3D11Buffer*)args->resource,args_offset);
}

void RenderDevice_DX11::Dispatch(UINT threadGruopCountX, UINT threadGruopCountY, UINT threadGruopCountZ)
{
	deviceContexts[ThreadID]->Dispatch(threadGruopCountX, threadGruopCountY, threadGruopCountZ);
}

void RenderDevice_DX11::DispatchIndirect(GPUBuffer* args, UINT args_offset)
{
	deviceContexts[ThreadID]->DispatchIndirect((ID3D11Buffer*)args, args_offset);
}

void RenderDevice_DX11::CopyTexture2D(Texture2D* pDest, Texture2D* pSrc)
{
	deviceContexts[ThreadID]->CopyResource((ID3D11Resource*)pDest->resource, (ID3D11Resource*)pSrc->resource);
}

void RenderDevice_DX11::CopyTexture2D_Region(Texture2D* pDest, UINT dstMip, UINT dstX, UINT dstY, Texture2D* pSrc, UINT srcMip)
{
	deviceContexts[ThreadID]->CopySubresourceRegion((ID3D11Resource*)pDest->resource, D3D11CalcSubresource(dstMip, 0, pDest->GetDesc().MipLevels), dstX, dstY, 0,
		(ID3D11Resource*)pSrc->resource, D3D11CalcSubresource(srcMip, 0, pSrc->GetDesc().MipLevels), nullptr);
}

void RenderDevice_DX11::MSAAResolve(Texture2D* pDst, Texture2D* pSrc)
{
	assert(pDst != nullptr && pSrc != nullptr);
	deviceContexts[ThreadID]->ResolveSubresource((ID3D11Resource*)pDst->resource, 0, (ID3D11Resource*)pSrc->resource, 0, ConvertFormat(pDst->desc.Format));
}

void RenderDevice_DX11::UpdateBuffer(GPUBuffer* buffer, const void* data, int datasize)
{
	assert(buffer->desc.Usage != USAGE_IMMUTABLE&&"Can not update IMMUTABLE GPUBuffer");
	assert((int)buffer->desc.ByteWidth >= datasize || datasize < 0 && "Data size is too big");

	if (datasize==0)
	{
		return;
	}
	datasize = min((int)buffer->desc.ByteWidth, datasize);

	if (buffer->desc.Usage==USAGE_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = deviceContexts[ThreadID]->Map((ID3D11Resource*)buffer->resource,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
		assert(SUCCEEDED(hr) && "GPUBuffer mapping failed!");
		memcpy(mappedResource.pData, data, (datasize >= 0? datasize : buffer->desc.ByteWidth));
		deviceContexts[ThreadID]->Unmap((ID3D11Resource*)buffer->resource, 0);
	}
	else if (buffer->desc.BindFlags & BIND_CONSTANT_BUFFER || datasize < 0)
	{
		deviceContexts[ThreadID]->UpdateSubresource((ID3D11Resource*)buffer->resource, 0, nullptr, data, 0, 0);
	}
	else
	{
		D3D11_BOX box = {};
		box.left = 0;
		box.back = 1;
		box.bottom = 1;
		box.front = 1;
		box.right = static_cast<UINT>(datasize);
		box.top = 0;
		deviceContexts[ThreadID]->UpdateSubresource((ID3D11Resource*)buffer->resource, 0, &box, data, 0, 0);
	}
}

void * RenderDevice_DX11::AllocateFromRingBuffer(GPURingBuffer* buffer, size_t dataSize, UINT& offsetIntoBuffer)
{
	assert(buffer->desc.Usage == USAGE_DYNAMIC && (buffer->desc.CPUAccessFlags &CPU_ACCESS_WRITE) && "Ringbuffer must be write");
	assert(buffer->desc.ByteWidth > dataSize && "Data of the required cannot fit");

	if (dataSize==0)
	{
		return nullptr;
	}

	dataSize = min(buffer->desc.ByteWidth, dataSize);

	size_t position = buffer->byteOffset;
	bool wrap = position + dataSize > buffer->desc.ByteWidth || buffer->residentFrame != FRAMECOUNT;
	position = wrap ? 0 : position;

	D3D11_MAP maping = wrap ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = deviceContexts[ThreadID]->Map((ID3D11Resource*)buffer->resource, 0, maping, 0, &mappedResource);
	assert(SUCCEEDED(hr) && "GPUBUFFER mapping failed!");

	buffer->byteOffset = position + dataSize;
	buffer->residentFrame = FRAMECOUNT;

	offsetIntoBuffer = (UINT)position;
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(mappedResource.pData) + position);

}

void RenderDevice_DX11::InvalidateBufferAccess(GPUBuffer* buffer)
{
	deviceContexts[ThreadID]->Unmap((ID3D11Resource*)buffer->resource, 0);
}

bool RenderDevice_DX11::DownloadResource(GPUBuffer* resurceToDownload, GPUResource* resourceDest, void* dataDest)
{
	{
		GPUBuffer* bufferToDownload = dynamic_cast<GPUBuffer*>(resurceToDownload);
		GPUBuffer* bufferDest=dynamic_cast<GPUBuffer*>(resourceDest);
		
		if (bufferToDownload != nullptr&& bufferDest != nullptr)
		{
			assert(bufferToDownload->desc.ByteWidth <= bufferDest->desc.ByteWidth);
			assert(bufferDest->desc.Usage& USAGE_STAGING);
			assert(dataDest != nullptr);

			deviceContexts[ThreadID]->CopyResource((ID3D11Resource*)bufferDest->resource, (ID3D11Resource*)bufferToDownload->resource);

			D3D11_MAPPED_SUBRESOURCE MappedSubresource = {};
			HRESULT hr = deviceContexts[ThreadID]->Map((ID3D11Resource*)bufferDest->resource, 0, D3D11_MAP_READ,/*asnyc* D3D11_MAP_FLAG_DO_NOT_WAIT*/ 0, &MappedSubresource);
			bool result = SUCCEEDED(hr);
			if (result)
			{
				memcpy(dataDest, MappedSubresource.pData, bufferToDownload->desc.ByteWidth);
				deviceContexts[ThreadID]->Unmap((ID3D11Resource*)bufferDest->resource,0);
			}
			return result;
		}
	}

	{
		Texture* textureToDownload = dynamic_cast<Texture*>(resurceToDownload);
		Texture* TextureDest = dynamic_cast<Texture*>(resourceDest);

		if (textureToDownload!=nullptr&&TextureDest!=nullptr)
		{
			assert(textureToDownload->desc.Width <= TextureDest->desc.Width);
			assert(textureToDownload->desc.Height <= TextureDest->desc.Height);
			assert(textureToDownload->desc.Depth <= TextureDest->desc.Depth);
			assert(TextureDest->desc.Usage& USAGE_STAGING);
			assert(dataDest != nullptr);

			deviceContexts[ThreadID]->CopyResource((ID3D11Resource*)TextureDest->resource, (ID3D11Resource*)textureToDownload->resource);

			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			HRESULT hr = deviceContexts[ThreadID]->Map((ID3D11Resource*)TextureDest->resource, 0, D3D11_MAP_READ, 0, &mappedResource);

			bool result = SUCCEEDED(hr);
			if (result)
			{
				UINT cpycount = max(1, textureToDownload->desc.Width)*max(1, textureToDownload->desc.Height)*max(1, textureToDownload->desc.Depth);
				UINT cpystride = GetFormatStride(textureToDownload->desc.Format);
				UINT cpysize = cpycount*cpystride;
				memcpy(dataDest, mappedResource.pData, cpysize);
				deviceContexts[ThreadID]->Unmap((ID3D11Resource*)TextureDest->resource, 0);
			}
			return result;
		}
	}
	return false;
}
void RenderDevice_DX11::WaitForGPU()
{
	deviceContexts[GRAPHICTHREAD_IMMERIATE]->Flush();
}

void RenderDevice_DX11::QueryBegin(GPUQuery *query)
{
	deviceContexts[ThreadID]->Begin((ID3D11Query*)query->resource[query->async_frameshift]);
	query->active[query->async_frameshift] = true;
}

void RenderDevice_DX11::QueryEnd(GPUQuery* query)
{
	deviceContexts[ThreadID]->End((ID3D11Query*)query->resource[query->async_frameshift]);
	query->active[query->async_frameshift] = true;
}

bool RenderDevice_DX11::QueryRead(GPUQuery* query)
{
	query->async_frameshift = (query->async_frameshift + 1) % (query->desc.async_Latnce + 1);
	const int readQueryID = query->async_frameshift;
	const UINT flag = (query->desc.async_Latnce > 0 ? D3D11_ASYNC_GETDATA_DONOTFLUSH : 0);

	if (!query->active[readQueryID])
	{
		return true;
	}

	assert(ThreadID == GRAPHICTHREAD_IMMERIATE && "A query can only be read on the immediate graphic thread!");

	HRESULT hr = S_OK;
	switch (query->desc.Type)
	{
	case PRE::GPU_QUERY_TYPE_TIMESTAMP:
		hr = deviceContexts[ThreadID]->GetData((ID3D11Query*)query->resource[readQueryID], &query->result_timestamp, sizeof(query->result_timestamp), flag);
		break;
	case  GPU_QUERY_TYPE_TIMESTAMP_DISJOINT:
		{
			D3D11_QUERY_DATA_TIMESTAMP_DISJOINT temp;
			hr = deviceContexts[ThreadID]->GetData((ID3D11Query*)query->resource[readQueryID], &temp, sizeof(temp), flag);
			query->result_disjoint = temp.Disjoint;
			query->result_timestamp = temp.Frequency;
		}
		break;
	case GPU_QUERY_TYPE_OCCLUSION:
		hr = deviceContexts[ThreadID]->GetData((ID3D11Query*)query->resource[readQueryID], &query->result_passed_sample_count, sizeof(query->result_passed_sample_count), flag);
		break;
	case GPU_QUERY_TYPE_OCCLUSION_PREDICATE:
	default:
		hr = deviceContexts[ThreadID]->GetData((ID3D11Query*)query->resource[readQueryID], &query->result_passed, sizeof(query->result_passed), flag);
		break;
	}

	query->active[readQueryID] = false;
	return hr != S_FALSE;
}

void RenderDevice_DX11::UAVBarrier(GPUResource* const* uavs, UINT NumBarries)
{

}

void RenderDevice_DX11::TransitionBarrier(GPUResource* const* resource, UINT NumBarrires, RESOUCRCE_STATES stateBefore, RESOUCRCE_STATES stateAFTER)
{

}



void RenderDevice_DX11::EventBegin(const std::string& name)
{
	userDefinedAnnotations[ThreadID]->BeginEvent(wstring(name.begin(), name.end()).c_str());
}

void RenderDevice_DX11::EventEnd()
{
	userDefinedAnnotations[ThreadID]->EndEvent();
}

void RenderDevice_DX11::SetMarker(const std::string &name)
{
	userDefinedAnnotations[ThreadID]->SetMarker(wstring(name.begin(), name.end()).c_str());
}

/*
void  RenderDevice_DX11::GetShaderReflection(GraphicBlob* blob, ShaderReflection* sf, Allocator* alloc)
{
	ShaderReflectionDX* sfdx=allocatorFC::allocateNew<ShaderReflectionDX>(*alloc);
	ID3D11ShaderReflection* pReflector = NULL;
	HRESULT hr = D3DReflect(((ID3D10Blob*)(blob->resourceDX))->GetBufferPointer(), ((ID3D10Blob*)(blob->resourceDX))->GetBufferSize(), IID_ID3D11ShaderReflection,
		(void**)&pReflector);

	if (hr)
	{
	}
    
	D3D11_SHADER_DESC desc;
	pReflector->GetDesc(&desc);
	sfdx->ShaderDescription = desc;

	for (UINT i=0;i<desc.InputParameters;++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC input_desc;
		pReflector->GetInputParameterDesc(i, &input_desc);
		//InputSignatureParameters.push_back(input_desc);
	}

	for (UINT i=0;i<desc.OutputParameters;++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC out_desc;
		pReflector->GetOutputParameterDesc(i, &out_desc);
	//	OutputSignatureParameters.push_back(out_desc);
	}

	for (UINT i=0;i<desc.ConstantBuffers;++i)
	{
		ID3D11ShaderReflectionConstantBuffer* pConstantBuffer = pReflector->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC bufferDesc;
		pConstantBuffer->GetDesc(&bufferDesc);
		
		if (bufferDesc.Type==D3D_CT_CBUFFER||bufferDesc.Type== D3D_CT_TBUFFER)
		{
			ConstantBufferLayoutDX* BufferLayout=allocatorFC::allocateNew<ConstantBufferLayoutDX>(*alloc);
			BufferLayout->Description = bufferDesc;
			D3D11_SHADER_INPUT_BIND_DESC bindesc;
			pReflector->GetResourceBindingDescByName(bufferDesc.Name,&bindesc);
			BufferLayout->BindPoint = std::move(bindesc.BindPoint);
			BufferLayout->BindCount = std::move(bindesc.BindCount);
			for (UINT j=0;j<BufferLayout->Description.Variables;++j)
			{
				ID3D11ShaderReflectionVariable* pVariable = pConstantBuffer->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC var_desc;
				pVariable->GetDesc(&var_desc);

				//BufferLayout.Variables.push_back(var_desc);

				ID3D11ShaderReflectionType* pType = pVariable->GetType();
				D3D11_SHADER_TYPE_DESC type_desc;
				pType->GetDesc(&type_desc);

				//BufferLayout.Types.push_back(std::move(type_desc));
				UINT bufferSize = PRE::AlignTo16(j, bufferDesc.Size);
				BufferLayout->pFirstContantsArray.push_back(std::move(bufferSize));
				BufferLayout->pContantsNumArray.push_back(std::move(bufferDesc.Size/4));
			}
			sfdx->ConstantBuffers.push_back(std::move(BufferLayout));
		}
	}

	for (UINT i=0;i<desc.BoundResources;++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC resource_desc;
		pReflector->GetResourceBindingDesc(i, &resource_desc);
		ShaderInputBindDescDX* binddesc=allocatorFC::allocateNew<ShaderInputBindDescDX>(*alloc,resource_desc);

		if (resource_desc.Type==D3D_SIT_CBUFFER||resource_desc.Type==D3D_SIT_TBUFFER)
		{
			//binddesc.pParamRef = pParamMgr->GetConstantBufferParameterRef( binddesc.Name );
		}
		else if (resource_desc.Type==D3D_SIT_TEXTURE||resource_desc.Type==D3D_SIT_STRUCTURED)
		{
			//binddesc.pParamRef = pParamMgr->GetShaderResourceParameterRef( binddesc.Name );
		}
		else if (resource_desc.Type == D3D_SIT_SAMPLER)
		{
			//binddesc.pParamRef = pParamMgr->GetSamplerStateParameterRef( binddesc.Name );
		}
		else if (resource_desc.Type == D3D_SIT_UAV_RWTYPED || resource_desc.Type == D3D_SIT_UAV_RWSTRUCTURED
			|| resource_desc.Type == D3D_SIT_BYTEADDRESS || resource_desc.Type == D3D_SIT_UAV_RWBYTEADDRESS
			|| resource_desc.Type == D3D_SIT_UAV_APPEND_STRUCTURED || resource_desc.Type == D3D_SIT_UAV_CONSUME_STRUCTURED
			|| resource_desc.Type == D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER)
		{
			//binddesc.pParamRef = pParamMgr->GetUnorderedAccessParameterRef( binddesc.Name );
		}
		sfdx->ResourceBindings.push_back(std::move(binddesc));
	}

	sf->ResourceDX = (CPUHandle)sfdx;
}*/
      
}

