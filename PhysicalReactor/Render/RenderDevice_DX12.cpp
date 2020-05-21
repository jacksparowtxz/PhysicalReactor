#include "RenderDevice_DX12.h"
#include "ResourceMapping.h"
#include "MISC/MiscHelper.h"

#include <sstream>

#include <functional>

#include <d3dcompiler.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"Dxgi.lib")
#pragma comment(lib,"dxguid.lib")

#ifdef PREDEBUG
#include<d3d12sdklayers.h>
#endif



using namespace std;
using namespace Microsoft::WRL;

namespace PRE
{


	inline UINT ParseColorWriteMask(UINT value)
	{
		UINT flag = 0;

		if (value == D3D12_COLOR_WRITE_ENABLE_ALL)
		{
			return D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		else
		{
			if (value & COLOR_WRITE_ENABLE_RED)
			{
				flag |= D3D12_COLOR_WRITE_ENABLE_RED;
			}
			if (value & COLOR_WRITE_ENABLE_GREEN)
			{
				flag |= D3D12_COLOR_WRITE_ENABLE_GREEN;
			}
			if (value & COLOR_WRITE_ENABLE_BLUE)
			{
				flag |= D3D12_COLOR_WRITE_ENABLE_BLUE;
			}
			if (value &COLOR_WRITE_ENABLE_ALPHA)
			{
				flag |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
			}
			return flag;
		}
	}

	inline D3D12_FILTER ConvertFilter(FILTER value)
	{
		switch (value)
		{
		case PRE::FILTER_MIN_MAG_MIP_POINT:
			return D3D12_FILTER_MIN_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MIN_MAG_POINT_MIP_LINEAR:
			return D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT:
			return D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MIN_POINT_MAG_MIP_LINEAR:
			return D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MIN_LINEAR_MAG_MIP_POINT:
			return D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			return D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MIN_MAG_LINEAR_MIP_POINT:
			return D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MIN_MAG_MIP_LINEAR:
			return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_ANISOTROPIC:
			return D3D12_FILTER_ANISOTROPIC;
			break;
		case PRE::FILTER_COMPARISON_MIN_MAG_MIP_POINT:
			return D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
			break;
		case PRE::FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
			return D3D12_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
			return D3D12_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
			return D3D12_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_COMPARSION_LINEAR_MAG_MIP_POINT:
			return D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case PRE::FILTER_COMPARSION_LINEAR_MAG_POINT_MIP_LINEAR:
			return D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_COMPARSION_MIN_MAG_LINEAR_MIP_POINT:
			return D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_COMPARSION_MIN_MAG_MIP_LINEAR:
			return D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_COMPARSION_ANISOTROPIC:
			return D3D12_FILTER_COMPARISON_ANISOTROPIC;
			break;
		case PRE::FILTER_MINIMUM_MIN_MAG_MIP_POINT:
			return D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MINMUM_MIN_MAG_POINT_MIP_LINEAR:
			return D3D12_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MINMUM_MIN_POINT_MAG_POINT_LINEAR_MIP_POINT:
			return D3D12_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MINMUM_MIN_POINT_MAG_MIP_LINEAR:
			return D3D12_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MINMUM_MIN_LINEAR_MAG_MIP_POINT:
			return D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MINMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			return D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MINMUM_MIN_MAG_LINEAR_MIP_POINT:
			return D3D12_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MINMUM_MIN_MAG_MIN_LINEAR:
			return D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MINMUM_ANSOTROPIC:
			return D3D12_FILTER_MINIMUM_ANISOTROPIC;
			break;
		case PRE::FILTER_MAXIMUM_MIN_MAG_MIP_POINT:
			return D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MAXIMUM_MIN_MAG_POINT_MIP_POINT:
			return D3D12_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT:
			return D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR:
			return D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT:
			return D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case PRE::FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
			return D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case PRE::FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT:
			return D3D12_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case PRE::FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR:
			return D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
			break;
		case PRE::FILTER_MAXIMUM_ANISOTROPIC:
			return D3D12_FILTER_MAXIMUM_ANISOTROPIC;
			break;
		default:
			break;
		}
		return D3D12_FILTER_MAXIMUM_ANISOTROPIC;
	}

	inline D3D12_TEXTURE_ADDRESS_MODE ConvertTextureAddressMode(TEXTURE_ADDRESS_MODE value)
	{
		switch (value)
		{
		case PRE::TEXTURE_ADDRESS_WRAP:
			return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			break;
		case PRE::TEXTURE_ADDRESS_MIRROR:
			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			break;
		case PRE::TEXTURE_ADDRESS_CLAMP:
			return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			break;
		case PRE::TEXTURE_ADDRESS_BORDER:
			return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			break;
		case PRE::TEXTURE_ADDRESS_MIRROR_ONCE:
			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
			break;
		default:
			break;
		}
		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	}

	inline D3D12_COMPARISON_FUNC ConvertComparisonFunc(COMPARSION_FUNC value)
	{
		switch (value)
		{
		case PRE::COMPARISON_NEVER:
			return D3D12_COMPARISON_FUNC_NEVER;
			break;
		case PRE::COMPARISON_LESS:
			return D3D12_COMPARISON_FUNC_LESS;
			break;
		case PRE::COMPARISON_EQUAL:
			return D3D12_COMPARISON_FUNC_EQUAL;
			break;
		case PRE::COMPARISON_LESS_EQUAL:
			return D3D12_COMPARISON_FUNC_LESS_EQUAL;
			break;
		case PRE::COMPARISON_GREATER:
			return D3D12_COMPARISON_FUNC_GREATER;
			break;
		case PRE::COMPARISON_NOT_EQUAL:
			return D3D12_COMPARISON_FUNC_NOT_EQUAL;
			break;
		case PRE::COMPARISON_GREATER_EQUAL:
			return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
			break;
		case PRE::COMPARISON_ALWAYS:
			return D3D12_COMPARISON_FUNC_ALWAYS;
			break;
		default:
			break;
		}
		return D3D12_COMPARISON_FUNC_NEVER;
	}

	inline D3D12_FILL_MODE ConvertFillMode(FILL_MODE value)
	{
		switch (value)
		{
		case PRE::FILL_WIREFRAME:
			return D3D12_FILL_MODE_WIREFRAME;
			break;
		case PRE::FILL_SOLID:
			return D3D12_FILL_MODE_SOLID;
			break;
		default:
			break;
		}
		return D3D12_FILL_MODE_WIREFRAME;
	}


	inline D3D12_CULL_MODE ConvertCullMode(CULL_MODE value)
	{
		switch (value)
		{
		case PRE::CULL_NONE:
			return D3D12_CULL_MODE_NONE;
			break;
		case PRE::CULL_FRONT:
			return D3D12_CULL_MODE_FRONT;
			break;
		case PRE::CULL_BACK:
			return D3D12_CULL_MODE_BACK;
			break;
		default:
			break;
		}
		return D3D12_CULL_MODE_NONE;
	}

	inline D3D12_DEPTH_WRITE_MASK ConvertDepthWriteMask(DEPTH_WRITE_MASK value)
	{
		switch (value)
		{
		case PRE::DEPTH_WRITE_MASK_ZERO:
			return D3D12_DEPTH_WRITE_MASK_ZERO;
			break;
		case PRE::DEPTH_WRITE_MASK_ALL:
			return D3D12_DEPTH_WRITE_MASK_ALL;
			break;
		default:
			break;
		}
		return D3D12_DEPTH_WRITE_MASK_ZERO;
	}

	inline D3D12_STENCIL_OP ConvertStencilOp(STENCIL_OP value)
	{
		switch (value)
		{
		case PRE::STENCIL_OP_KEEP:
			return D3D12_STENCIL_OP_KEEP;
			break;
		case PRE::STENCIL_OP_ZERO:
			return D3D12_STENCIL_OP_ZERO;
			break;
		case PRE::STENCIL_OP_REPLACE:
			return D3D12_STENCIL_OP_REPLACE;
			break;
		case PRE::STENCIL_OP_INCR_SAT:
			return D3D12_STENCIL_OP_INCR_SAT;
			break;
		case PRE::STENCIL_OP_DECR_SAT:
			return D3D12_STENCIL_OP_DECR_SAT;
			break;
		case PRE::STENCIL_OP_INVERT:
			return D3D12_STENCIL_OP_INVERT;
			break;
		case PRE::STENCIL_OP_INCR:
			return D3D12_STENCIL_OP_INCR;
			break;
		case PRE::STENCIL_OP_DECR:
			return D3D12_STENCIL_OP_DECR;
			break;
		default:
			break;
		}

		return D3D12_STENCIL_OP_KEEP;
	}

	inline D3D12_BLEND ConvertBlend(BLEND value)
	{
		switch (value)
		{
		case PRE::BLEND_ZERO:
			return D3D12_BLEND_ZERO;
			break;
		case PRE::BLEND_ONE:
			return D3D12_BLEND_ONE;
			break;
		case PRE::BLEND_SRC_COLOR:
			return D3D12_BLEND_SRC_COLOR;
			break;
		case PRE::BLEND_INV_SRC_COLOR:
			return D3D12_BLEND_INV_SRC_COLOR;
			break;
		case PRE::BLEND_SRC_ALPHA:
			return D3D12_BLEND_SRC_ALPHA;
			break;
		case PRE::BLEND_INV_SRC_ALPHA:
			return D3D12_BLEND_INV_SRC_ALPHA;
			break;
		case PRE::BLEND_DEST_ALPHA:
			return D3D12_BLEND_DEST_ALPHA;
			break;
		case PRE::BLEND_INV_DEST_ALPHA:
			return D3D12_BLEND_INV_DEST_ALPHA;
			break;
		case PRE::BLEND_DEST_COLOR:
			return D3D12_BLEND_DEST_COLOR;
			break;
		case PRE::BLEND_INV_DEST_COLOR:
			return D3D12_BLEND_INV_DEST_COLOR;
			break;
		case PRE::BLEND_SRC_ALPHA_SAT:
			return D3D12_BLEND_SRC_ALPHA_SAT;
			break;
		case PRE::BLEND_BLEND_FACTOR:
			return D3D12_BLEND_BLEND_FACTOR;
			break;
		case PRE::BLEND_INV_BLEND_COLOR:
			return D3D12_BLEND_INV_BLEND_FACTOR;
			break;
		case PRE::BLEND_SRC1_COLOR:
			return D3D12_BLEND_SRC1_COLOR;
			break;
		case PRE::BLEND_INV_SRC1_COLOR:
			return D3D12_BLEND_INV_SRC1_COLOR;
			break;
		case PRE::BLEND_SRC1_ALPHA:
			return D3D12_BLEND_SRC1_ALPHA;
			break;
		case PRE::BLEND_INV_SRC1_ALPHA:
			return D3D12_BLEND_INV_SRC1_ALPHA;
			break;
		default:
			break;
		}
		return D3D12_BLEND_ZERO;
	}

	inline D3D12_BLEND_OP ConvertBlendOp(BLEND_OP value)
	{
		switch (value)
		{
		case PRE::BLEND_OP_ADD:
			return D3D12_BLEND_OP_ADD;
			break;
		case PRE::BLEND_OP_SUBTRACT:
			return D3D12_BLEND_OP_SUBTRACT;
			break;
		case PRE::BLEND_OP_REV_SUBTRACT:
			return D3D12_BLEND_OP_REV_SUBTRACT;
			break;
		case PRE::BLEND_OP_MIN:
			return D3D12_BLEND_OP_MIN;
			break;
		case PRE::BLEND_OP_MAX:
			return D3D12_BLEND_OP_MAX;
			break;
		default:
			break;
		}
		return D3D12_BLEND_OP_ADD;
	}


	inline D3D12_INPUT_CLASSIFICATION ConvertInputClassification(INPUT_CLASSIFICATION value)
	{
		switch (value)
		{
		case PRE::INPUT_PER_VERTEX_DATA:
			return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			break;
		case PRE::INPUT_PER_INSTANCE_DATA:
			return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
			break;
		default:
			break;
		}
		return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
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






	inline D3D12_SUBRESOURCE_DATA ConvertSubresourceData(const SubresourceData& pInitialData)
	{
		D3D12_SUBRESOURCE_DATA data;
		data.pData= pInitialData.pSysMem;
		data.RowPitch = pInitialData.SysMemPitch;
		data.SlicePitch = pInitialData.SysMemSlicePitch;

		return data;
	}

	constexpr D3D12_RESOURCE_STATES ConvertImageLayout(IMAGE_LAYOUT value)
	{
		switch (value)
		{
		case PRE::IMAGE_LAYOUT_UNDEFINED:
		case PRE::IMAGE_LAYOUT_GENERAL:
			return D3D12_RESOURCE_STATE_COMMON;
		case PRE::IMAGE_LAYOUT_RENDERTRGET:
			return D3D12_RESOURCE_STATE_RENDER_TARGET;
		case PRE::IMAGE_LAYOUT_DEPTHSTENCIL:
			return D3D12_RESOURCE_STATE_DEPTH_WRITE;
		case PRE::IMAGE_LAYOUT_DEPTHSTENCIL_READONLY:
			return D3D12_RESOURCE_STATE_DEPTH_READ;
		case PRE::IMAGE_LAYOUT_SHADER_RESOURCE:
			return D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		case PRE::IMAGE_LAYOUT_UNORDERED_ACCESS:
			return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		case PRE::IMAGE_LAYOUT_COPY_SRC:
			return D3D12_RESOURCE_STATE_COPY_SOURCE;
		case PRE::IMAGE_LAYOUT_COPY_DEST:
			return D3D12_RESOURCE_STATE_COPY_DEST;
		}

		return D3D12_RESOURCE_STATE_COMMON;
	   
	}

	constexpr D3D12_RESOURCE_STATES ConvertBufferState(BUFFER_STATE value)
	{
		switch (value)
		{
		case PRE::BUFFER_STATE_GENERAL:
			return D3D12_RESOURCE_STATE_COMMON;
		case PRE::BUFFER_STATE_VERTEX_BUFFER:
			return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		case PRE::BUFFER_STATE_INDEX_BUFFER:
			return D3D12_RESOURCE_STATE_INDEX_BUFFER;
		case PRE::BUFFER_STATE_CONSTANT_BUFFER:
			return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		case PRE::BUFFER_STATE_INDIRECT_ARGUMENT:
			return D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
		case PRE::BUFFER_STATE_SHADER_RESOURCE:
			return D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		case PRE::BUFFER_STATE_UNORDERED_ACCESS:
			return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		case PRE::BUFFER_STATE_COPY_SRC:
			return D3D12_RESOURCE_STATE_COPY_SOURCE;
		case PRE::BUFFER_STATE_COPY_DST:
			return D3D12_RESOURCE_STATE_COPY_DEST;
		}

		return D3D12_RESOURCE_STATE_COMMON;
	
	
	
	}


	

	constexpr FORMAT ConvertFormat_Inv(DXGI_FORMAT value)
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


constexpr TextureDesc ConvertTextureDesc_Inv(const D3D12_RESOURCE_DESC& desc)
{
	TextureDesc retVal;

	retVal.Format = ConvertFormat_Inv(desc.Format);
	retVal.Width = (uint32_t)desc.Width;
	retVal.Height = desc.Height;
	retVal.MipLevels = desc.MipLevels;

	return retVal;
}



const void* const nullBlob[1024] = {};

inline size_t Align(size_t location, size_t align)
{
	if ((0 == align) || (align&(align - 1)))
	{
		assert(0);
	}
	return ((location + (align - 1)) & ~(align - 1));
}

struct Resource_DX12
{
	std::shared_ptr<RenderDevice_DX12::AllocationHandler> allocationhandler;
	D3D12MA::Allocation* allocation = nullptr;
	ComPtr<ID3D12Resource1> resource;
	D3D12_CPU_DESCRIPTOR_HANDLE cbv;
	D3D12_CPU_DESCRIPTOR_HANDLE srv;
	D3D12_CPU_DESCRIPTOR_HANDLE uav;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> subresources_srv;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> subresources_uav;

	virtual ~Resource_DX12()
	{
		allocationhandler->destroylocker.lock();
		uint64_t framecount = allocationhandler->framecount;
		if (allocation) allocationhandler->destoryer_allocations.push_back(std::make_pair(allocation, framecount));
		if (resource) allocationhandler->destroyer_resources.push_back(std::make_pair(resource, framecount));
		if (cbv.ptr) allocationhandler->destroyer_resourceviews.push_back(std::make_pair(cbv, framecount));
		if (srv.ptr) allocationhandler->destroyer_resourceviews.push_back(std::make_pair(srv, framecount));
		if (uav.ptr) allocationhandler->destroyer_resourceviews.push_back(std::make_pair(uav, framecount));
		for (auto x : subresources_srv)
		{
			allocationhandler->destroyer_resourceviews.push_back(std::make_pair(x, framecount));
		}
		for (auto x : subresources_uav)
		{
			allocationhandler->destroyer_resourceviews.push_back(std::make_pair(x, framecount));
		}
		allocationhandler->destroylocker.unlock();

	}
};

struct Texture_DX12 : public Resource_DX12
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtv;
	D3D12_CPU_DESCRIPTOR_HANDLE dsv;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> subresources_rtv;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> subresources_dsv;

	~Texture_DX12() override
	{
		allocationhandler->destroylocker.lock();
		uint64_t framecount = allocationhandler->framecount;
		if (rtv.ptr) allocationhandler->destroyer_rtvs.push_back(std::make_pair(rtv, framecount));
		if (dsv.ptr) allocationhandler->destroyer_dsvs.push_back(std::make_pair(dsv, framecount));
		for (auto x : subresources_rtv)
		{
			allocationhandler->destroyer_rtvs.push_back(std::make_pair(x, framecount));
		}
		for (auto x : subresources_dsv)
		{
			allocationhandler->destroyer_dsvs.push_back(std::make_pair(x, framecount));
		}
		allocationhandler->destroylocker.unlock();
	}
};

struct Sampler_DX12
{
	std::shared_ptr<RenderDevice_DX12::AllocationHandler> allocationhandler;
	D3D12_CPU_DESCRIPTOR_HANDLE descriptor;

	~Sampler_DX12()
	{
		allocationhandler->destroylocker.lock();
		uint64_t framecount = allocationhandler->framecount;
		if (descriptor.ptr) allocationhandler->destroyer_samplers.push_back(std::make_pair(descriptor, framecount));
		allocationhandler->destroylocker.unlock();
	}
};

struct Query_DX12
{
	std::shared_ptr<RenderDevice_DX12::AllocationHandler> allocationhandler;
	GPU_QUERY_TYPE query_type = GPU_QUERY_TYPE_INVALID;
	uint32_t query_index = ~0;

	~Query_DX12()
	{
		if (query_index != ~0)
		{
			allocationhandler->destroylocker.lock();
			uint64_t framecount = allocationhandler->framecount;
			switch (query_type)
			{
			case PRE::GPU_QUERY_TYPE_OCCLUSION:
			case PRE::GPU_QUERY_TYPE_OCCLUSION_PREDICATE:
				allocationhandler->destroyer_queries_occlusion.push_back(std::make_pair(query_index, framecount));
				break;
			case PRE::GPU_QUERY_TYPE_TIMESTAMP:
				allocationhandler->destroyer_queries_timestamp.push_back(std::make_pair(query_index, framecount));
				break;
			}
			allocationhandler->destroylocker.unlock();
		}
	}
};



struct PipelineState_DX12
{
	std::shared_ptr<RenderDevice_DX12::AllocationHandler> allocationhandler;
	ComPtr<ID3D12PipelineState> resource;

	~PipelineState_DX12()
	{
		allocationhandler->destroylocker.lock();
		uint64_t framecount = allocationhandler->framecount;
		if (resource) allocationhandler->destroyer_pipelines.push_back(std::make_pair(resource, framecount));
		allocationhandler->destroylocker.unlock();
	}
};

Resource_DX12* to_internal(const GPUResource* param)
{
	return static_cast<Resource_DX12*>(param->internal_state.get());
}
Resource_DX12* to_internal(const GPUBuffer* param)
{
	return static_cast<Resource_DX12*>(param->internal_state.get());
}
Texture_DX12* to_internal(const Texture* param)
{
	return static_cast<Texture_DX12*>(param->internal_state.get());
}
Sampler_DX12* to_internal(const Sampler* param)
{
	return static_cast<Sampler_DX12*>(param->internal_state.get());
}
Query_DX12* to_internal(const GPUQuery* param)
{
	return static_cast<Query_DX12*>(param->internal_state.get());
}
PipelineState_DX12* to_internal(const Shader* param)
{
	assert(param->stage == CS_STAGE); // only compute shader has pipeline state!
	return static_cast<PipelineState_DX12*>(param->internal_state.get());
}
PipelineState_DX12* to_internal(const PipelineState* param)
{
	return static_cast<PipelineState_DX12*>(param->internal_state.get());
}



void RenderDevice_DX12::DescriptorAllocator::init(ID3D12Device6* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t maxCount)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = maxCount;
	heapDesc.Type = type;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
	assert(SUCCEEDED(hr));

	heap_begin = heap->GetCPUDescriptorHandleForHeapStart().ptr;
	itemSize = device->GetDescriptorHandleIncrementSize(type);
	itemCount = 0;
	this->maxCount = maxCount;

	itemAlive = new bool[maxCount];
	for (uint32_t i = 0; i < maxCount; ++i)
	{
		itemAlive[i] = false;
	}
	lastAlloc = 0;
}

RenderDevice_DX12::DescriptorAllocator::~DescriptorAllocator()
{
	delete[] itemAlive;
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderDevice_DX12::DescriptorAllocator::allocate()
{
	D3D12_CPU_DESCRIPTOR_HANDLE addr = {};

	lock.lock();
	if (itemCount < maxCount - 1)
	{
		while (itemAlive[lastAlloc] == true)
		{
			lastAlloc = (lastAlloc + 1) % maxCount;
		}
		addr.ptr = heap_begin + lastAlloc * itemSize;
		itemAlive[lastAlloc] = true;

		itemCount++;
	}
	else
	{
		assert(0);
	}
	lock.unlock();
	return addr;
}

RenderDevice_DX12::RenderDevice_DX12(HWND mainscreen,bool fullscreen, bool debuglayer):RenderDevice()
{
	FULLSCREEN = fullscreen;

	

	RECT rect = RECT();
	GetClientRect(mainscreen, &rect);

	SCREENWIDTH = rect.right-rect.left;
	SCREENHEIGHT = rect.bottom - rect.top;



	HRESULT hr = E_FAIL;
	for (int i=0;i<JobScheduler::NumThreads;i++)
	{
		SAFE_INITIL(commandlists[i]);
		SAFE_INITIL(deviceContexts[i]);
		stencilRef[i] = 0;
		blendFactor[i] = XMFLOAT4(1, 1, 1, 1);
	}

	UINT createDeviceFlags = 0;

	if (debuglayer)
	{
		createDeviceFlags |= D3D12_CREATE_DEVICE_DEBUG;
	}

	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0 ,D3D_FEATURE_LEVEL_10_1 };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex=0;driverTypeIndex<numDriverTypes;driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D12CreateDevice(nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D12_SDK_VERSION, &Basedevice, &featureLevel, &BasedeviceContexts);
#ifdef PREDEBUG
		static const char c_szName[] = "Basedevice";
		 Basedevice->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(c_szName) - 1, c_szName);
#endif // PREDEBUG
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
		hr = Basedevice->QueryInterface(__uuidof(ID3D12Device3), reinterpret_cast<void**>(&device));
		if (SUCCEEDED(hr))
		{
		   hr=BasedeviceContexts->QueryInterface(__uuidof(ID3D12DeviceContext3), reinterpret_cast<void**>(&ImmediatedeviceContext));
		}
	}
#ifdef PREDEBUG
	static const char c_szName1[] = "device";
	static const char c_szName2[] = "BasedeviceContexts";
	device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(c_szName1) - 1, c_szName1);
	device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(c_szName2) - 1, c_szName2);
#endif // PREDEBUG
	Basedevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, 4, &M4XMSAAQUALITY);

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
#ifdef PREDEBUG
	static const char c_szName3[] = "swapChain";
	swapChain->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(c_szName3) - 1, c_szName3);
#endif // PREDEBUG
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
#ifdef PREDEBUG
	static const char c_szName4[] = "ImmediatedeviceContext";
	ImmediatedeviceContext->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(c_szName4) - 1, c_szName4);
#endif // PREDEBUG
	D3D12_FEATURE_DATA_D3D12_OPTIONS2 features_2;
	hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &features_2, sizeof(features_2));//
	CONSERVATIVE_RASTERIZATION = features_2.ConservativeRasterizationTier >= D3D12_CONSERVATIVE_RASTERIZATION_TIER_1;
	RASTERIZER_ORDERED_VIEWS = features_2.ROVsSupported == TRUE;
	UNORDEREDACCESSTEXTURE_LOAD_EXT = features_2.TypedUAVLoadAdditionalFormats == TRUE;

	D3D12_FEATURE_DATA_THREADING threadingFeature;
	device->CheckFeatureSupport(D3D12_FEATURE_THREADING, &threadingFeature, sizeof(threadingFeature));
	if (threadingFeature.DriverConcurrentCreates && threadingFeature.DriverCommandLists)
	{
		MULTITHREAD_RENDERING = true;
		for (int i=0;i<JobScheduler::NumThreads;i++)
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
	SAFE_RELEASE(offscreenRTV);
	SAFE_RELEASE(offscreenDSV);
	SAFE_RELEASE(backbuffer);
	SAFE_RELEASE(depthbuffer);

	for (int i=0;i<JobScheduler::NumThreads;i++)
	{
		SAFE_RELEASE(commandlists[i]);
		SAFE_RELEASE(userDefinedAnnotations[i]);
		SAFE_RELEASE(deviceContexts[i]);
		SAFE_RELEASE(prev_vs[i]);
		SAFE_RELEASE(prev_ps[i]);
		SAFE_RELEASE(prev_hs[i]);
		SAFE_RELEASE(prev_ds[i]);
		SAFE_RELEASE(prev_gs[i]);
		SAFE_RELEASE(prev_bs[i]);
		SAFE_RELEASE(prev_rs[i]);
		SAFE_RELEASE(prev_dss[i]);
		SAFE_RELEASE(prev_vl[i]);
	}
	SAFE_RELEASE(BasedeviceContexts);
	SAFE_RELEASE(ImmediatedeviceContext);;
	ID3D12Debug* d3dDebug;
	HRESULT hr = device->QueryInterface(__uuidof(ID3D12Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		hr = d3dDebug->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
	}
	if (d3dDebug != nullptr)
		d3dDebug->Release();
	SAFE_RELEASE(device);
	SAFE_RELEASE(Basedevice);
	
}

void RenderDevice_DX11::SetResolution(int width, int height)
{
	D3D12_VIEWPORT ScreenViewport;
	ScreenViewport.TopLeftX = 0;
	ScreenViewport.TopLeftY = 0;
	ScreenViewport.Width = static_cast<float>(width);
	ScreenViewport.Height = static_cast<float>(height);

	ScreenViewport.MinDepth = 0.0f;
	ScreenViewport.MaxDepth = 1.0f;
	
	for (int i = 0; i < JobScheduler::NumThreads; i++)
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
	
	

	/*for (int i = 0; i < JobScheduler::NumThreads; i++)
	{
		deviceContexts[i]->OMSetRenderTargets(1, &renderTargetView, DepthStecilView);
	}*/
}
	
Texture2D RenderDevice_DX11::GetBackBuffer()
{
	Texture2D result;
	result.resource = (CPUHandle)backbuffer;
	backbuffer->AddRef();

	D3D12_TEXTURE2D_DESC desc;
	backbuffer->GetDesc(&desc);
	result.desc = ConvertTextureDesc_Inv(&desc);

	return result;
}



HRESULT RenderDevice_DX11::CreateBuffer(const GPUBufferDesc *pDesc, const SubresourceData* pInitialData, GPUBuffer *ppBuffer)
{
	ppBuffer->Register(this);
	D3D12_BUFFER_DESC desc;
	desc.ByteWidth = pDesc->ByteWidth;
	desc.Usage = ConvertUsage(pDesc->Usage);
	desc.BindFlags = ParseBindFlags(pDesc->BindFlags);
	desc.CPUAccessFlags = ParseCPUAccessFlags(pDesc->CPUAccessFlags);
	desc.MiscFlags = ParseResourceMiscFlags(pDesc->MiscFlags);
	desc.StructureByteStride = pDesc->StructureByteStride;

	D3D12_SUBRESOURCE_DATA* data = nullptr;
	if (pInitialData!=nullptr)
	{
		data = new D3D12_SUBRESOURCE_DATA[1];
		for (UINT slice=0;slice<1;++slice)
		{
			data[slice] = ConvertSubresourceData(pInitialData[slice]);
		}
	}

	ppBuffer->desc = *pDesc;
	HRESULT hr = device->CreateBuffer(&desc, data,(ID3D12Buffer**)&ppBuffer->resource);
	SAFE_DELETE_ARRAY(data);
	assert(SUCCEEDED(hr) && "GPUBuffer creation failed!");
	
	if (SUCCEEDED(hr))
	{
		if (desc.BindFlags & D3D12_BIND_SHADER_RESOURCE)
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc;
			ZeroMemory(&srv_desc, sizeof(srv_desc));

			if (desc.MiscFlags & D3D12_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
			{
				srv_desc.Format = DXGI_FORMAT_R32_TYPELESS;
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFEREX;
				srv_desc.BufferEx.FirstElement = 0;
				srv_desc.BufferEx.Flags = D3D12_BUFFEREX_SRV_FLAG_RAW;
				srv_desc.BufferEx.NumElements = desc.ByteWidth / 4;
			}
			else if (desc.MiscFlags & D3D12_RESOURCE_MISC_BUFFER_STRUCTURED)
			{
				srv_desc.Format = DXGI_FORMAT_UNKNOWN;
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFEREX;
				srv_desc.BufferEx.FirstElement = 0;
				srv_desc.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
			}
			else
			{
				srv_desc.Format = ConvertFormat(pDesc->Format);
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFEREX;
				srv_desc.Buffer.FirstElement = 0;
				srv_desc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
			}
			hr = device->CreateShaderResourceView((ID3D12Resource*)ppBuffer->resource, &srv_desc, (ID3D12ShaderResourceView**)&ppBuffer->SRV);

			assert(SUCCEEDED(hr) && "ShaderResourceView of the GPUBuffer could not be created!");
		}
		if (desc.BindFlags & D3D12_BIND_UNORDERED_ACCESS)
		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			ZeroMemory(&uav_desc, sizeof(uav_desc));
			uav_desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			uav_desc.Buffer.FirstElement = 0;

			if (desc.MiscFlags & D3D12_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
			{
				uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
				uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
				uav_desc.Buffer.NumElements = desc.ByteWidth / 4;
			}
			else if (desc.MiscFlags & D3D12_RESOURCE_MISC_BUFFER_STRUCTURED)
			{
				uav_desc.Format = DXGI_FORMAT_UNKNOWN;
				uav_desc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
			}
			else
			{
				uav_desc.Format = ConvertFormat(pDesc->Format);
				uav_desc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
			}

			hr = device->CreateUnorderedAccessView((ID3D12Resource*)ppBuffer->resource, &uav_desc, (ID3D12UnorderedAccessView**)&ppBuffer->UAV);
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

	 D3D12_TEXTURE1D_DESC desc = ConvertTextureDesc1D(&(*ppTexture1D)->desc);

	 D3D12_SUBRESOURCE_DATA* data = nullptr;
	 if (pInitialData!=nullptr)
	 {
		 data = new D3D12_SUBRESOURCE_DATA[pDesc->ArraySize];
		 for (UINT slice=0;slice<pDesc->ArraySize;++slice)
		 {
			 data[slice] = ConvertSubresourceData(pInitialData[slice]);
		 }
	 }

	 HRESULT hr = S_OK;
	 hr = device->CreateTexture1D(&desc, data, (ID3D12Texture1D**)&((*ppTexture1D)->resource));
	 SAFE_DELETE_ARRAY(data);
	assert(SUCCEEDED(hr) && "Texture1D creation failed!");
	 if (FAILED(hr))
		 return hr;

	 if ((*ppTexture1D)->desc.MipLevels==0)
	 {
	    (*ppTexture1D)->desc.MipLevels = (UINT)log2((*ppTexture1D)->desc.Width);
	 }

	 CreateShaderResourceView(*ppTexture1D);
	 if (desc.BindFlags & D3D12_BIND_UNORDERED_ACCESS)
	 {
		 assert((*ppTexture1D)->independentRTVArraySlices == false && "TextureArray UAV not implemnted");

		 D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc;
		 ZeroMemory(&uav_desc, sizeof(uav_desc));
		 uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
		 uav_desc.Texture2D.MipSlice = 0;

		 hr = device->CreateUnorderedAccessView((ID3D12Resource*)(*ppTexture1D)->resource, &uav_desc, (ID3D12UnorderedAccessView**)&(*ppTexture1D)->UAV);
		 assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture1D could not be created!");
	 }

	 return hr;
}



HRESULT RenderDevice_DX11::CreateTexture2D(const TextureDesc * pDesc, const SubresourceData * pInitialData, Texture2D ** ppTexture2D)
{
	if ((*ppTexture2D) == nullptr)
	{
		(*ppTexture2D) = new Texture2D;
	}
	(*ppTexture2D)->Register(this);

	(*ppTexture2D)->desc = *pDesc;

	if ((*ppTexture2D)->desc.SampleDesc.Count > 1)
	{
		UINT qulity;
		device->CheckMultisampleQualityLevels(ConvertFormat((*ppTexture2D)->desc.Format), (*ppTexture2D)->desc.SampleDesc.Count, &qulity);
		(*ppTexture2D)->desc.SampleDesc.Quality = qulity - 1;
		if (qulity == 0)
		{
			assert(0 && "MSAA Samplecount not supported!");
			(*ppTexture2D)->desc.SampleDesc.Count = 1;
		}
	}

	D3D12_TEXTURE2D_DESC desc = ConvertTextureDesc2D(&(*ppTexture2D)->desc);


	D3D12_SUBRESOURCE_DATA* data = nullptr;
	if (pInitialData != nullptr)
	{
		UINT dataCount = pDesc->ArraySize* max(1, pDesc->MipLevels);
		data = new D3D12_SUBRESOURCE_DATA[dataCount];
		for (UINT slice = 0; slice < dataCount; ++slice)
		{
			data[slice] = ConvertSubresourceData(pInitialData[slice]);
		}
	}

	HRESULT hr = S_OK;

	hr = device->CreateTexture2D(&desc, data, (ID3D12Texture2D**)&((*ppTexture2D)->resource));
	assert(SUCCEEDED(hr) && "Texture2D creation failed!");
	SAFE_DELETE_ARRAY(data);
	if (FAILED(hr))
		return hr;

	if ((*ppTexture2D)->desc.MipLevels == 0)
	{
		(*ppTexture2D)->desc.MipLevels = (UINT)log2(max((*ppTexture2D)->desc.Width, (*ppTexture2D)->desc.Height));
	}

	CreateRenderTargetView(*ppTexture2D);
	CreateShaderResourceView(*ppTexture2D);
	CreateDepthStencilView(*ppTexture2D);
	if (desc.BindFlags & D3D12_BIND_UNORDERED_ACCESS)
	{
		if (desc.ArraySize > 1)
		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			ZeroMemory(&uav_desc, sizeof(uav_desc));
			uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
			uav_desc.Texture2DArray.FirstArraySlice = 0;
			uav_desc.Texture2DArray.ArraySize = desc.ArraySize;
			uav_desc.Texture2DArray.MipSlice = 0;

			if ((*ppTexture2D)->indepentdentUAVMIPs)
			{
				UINT miplevels = (*ppTexture2D)->desc.MipLevels;
				for (UINT i = 0; i < miplevels; ++i)
				{
					uav_desc.Texture2DArray.MipSlice = i;

					(*ppTexture2D)->additionalUAVs.push_back(NUll_Handle);
					hr = device->CreateUnorderedAccessView((ID3D12Resource*)(*ppTexture2D)->resource, &uav_desc, (ID3D12UnorderedAccessView**)&(*ppTexture2D)->additionalUAVs[i]);
					assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture2D could not be created!");
				}
			}
			{
				uav_desc.Texture2DArray.MipSlice = 0;
				hr = device->CreateUnorderedAccessView((ID3D12Resource*)(*ppTexture2D)->resource, &uav_desc, (ID3D12UnorderedAccessView**)&(*ppTexture2D)->UAV);
			}
		}
		else
		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			ZeroMemory(&uav_desc, sizeof(uav_desc));
			uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

			if ((*ppTexture2D)->indepentdentUAVMIPs)
			{
				UINT miplevels = (*ppTexture2D)->desc.MipLevels;
				for (UINT i = 0; i < miplevels; ++i)
				{
					uav_desc.Texture2D.MipSlice = i;

					(*ppTexture2D)->additionalUAVs.push_back(NUll_Handle);
					hr = device->CreateUnorderedAccessView((ID3D12Resource*)(*ppTexture2D)->resource, &uav_desc, (ID3D12UnorderedAccessView**)&(*ppTexture2D)->additionalUAVs[i]);
					assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture2D could not be created!");
				}
			}
			{
				uav_desc.Texture2D.MipSlice = 0;
				hr = device->CreateUnorderedAccessView((ID3D12Resource*)(*ppTexture2D)->resource, &uav_desc, (ID3D12UnorderedAccessView**)&(*ppTexture2D)->UAV);
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

	D3D12_TEXTURE3D_DESC desc = ConvertTextureDesc3D(&(*ppTexture3D)->desc);

	D3D12_SUBRESOURCE_DATA* data = nullptr;
	if (pInitialData != nullptr)
	{
		data = new D3D12_SUBRESOURCE_DATA[1];
		for (UINT slice = 0; slice < 1; ++slice)
		{
			data[slice] = ConvertSubresourceData(pInitialData[slice]);
		}
	}

	HRESULT hr = S_OK;

	hr = device->CreateTexture3D(&desc, data, (ID3D12Texture3D**)&((*ppTexture3D)->resource));
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

	if (desc.BindFlags & D3D12_BIND_UNORDERED_ACCESS)
	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc;
		ZeroMemory(&uav_desc, sizeof(uav_desc));
		uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
		uav_desc.Texture3D.FirstWSlice = 0;

		if ((*ppTexture3D)->indepentdentUAVMIPs)
		{
			UINT miplevels = (*ppTexture3D)->desc.MipLevels;
			uav_desc.Texture3D.WSize = desc.Depth;
			for (UINT i = 0; i < miplevels; ++i)
			{
				uav_desc.Texture3D.MipSlice = i;

				(*ppTexture3D)->additionalUAVs.push_back(NUll_Handle);
				hr = device->CreateUnorderedAccessView((ID3D12Resource*)(*ppTexture3D)->resource, &uav_desc, (ID3D12UnorderedAccessView**)&(*ppTexture3D)->additionalUAVs[i]);
				assert(SUCCEEDED(hr) && "UnorderedAccessView of the Texture3D could not be created!");
				uav_desc.Texture3D.WSize /= 2;
			}
		}

		{
			uav_desc.Texture3D.MipSlice = 0;
			uav_desc.Texture3D.WSize = desc.Depth;
			hr = device->CreateUnorderedAccessView((ID3D12Resource*)(*ppTexture3D)->resource, &uav_desc, (ID3D12UnorderedAccessView**)&(*ppTexture3D)->UAV);
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

		D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = ConvertFormat(pTexture->desc.Format);

		if (arraySize>1)
		{
			shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
			shaderResourceViewDesc.Texture1DArray.FirstArraySlice = 0;
			shaderResourceViewDesc.Texture1DArray.ArraySize = arraySize;
			shaderResourceViewDesc.Texture1DArray.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture1DArray.MipLevels = -1;
		}
		else
		{
			shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
			shaderResourceViewDesc.Texture1D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture1D.MipLevels = -1;
		}

		hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->SRV);

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

		 D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
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
					 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
					 shaderResourceViewDesc.TextureCubeArray.First2DArrayFace = 0;
					 shaderResourceViewDesc.TextureCubeArray.NumCubes = arraysize;
					 shaderResourceViewDesc.TextureCubeArray.MostDetailedMip = 0;
					 shaderResourceViewDesc.TextureCubeArray.MipLevels = -1;
				 }
				 else
				 {
					 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
					 shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
					 shaderResourceViewDesc.TextureCube.MipLevels = -1;
				 }
			 }
			 else
			 {
				 if (multisampled)
				 {
					 shaderResourceViewDesc.ViewDimension= D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
					 shaderResourceViewDesc.Texture2DMSArray.FirstArraySlice = 0;
					 shaderResourceViewDesc.Texture2DMSArray.ArraySize = arraysize;
				 }
				 else
				 {
					 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
					 shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
					 shaderResourceViewDesc.Texture2DArray.ArraySize = arraysize;
					 shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
					 shaderResourceViewDesc.Texture2DArray.MipLevels = -1;
				 }
			 }
			 hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->SRV);
			 assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");

			 if (pTexture->indepentdentSRVMIPs)
			 {
				 if (pTexture->desc.MiscFlags & RESOURCE_MISC_TEXTURECUBE)
				 {
					 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
					 shaderResourceViewDesc.TextureCubeArray.First2DArrayFace = 0;
					 shaderResourceViewDesc.TextureCubeArray.NumCubes = arraysize / 6;

					 for (UINT j=0;j<pTexture->desc.MipLevels;++j)
					 {
						 shaderResourceViewDesc.TextureCubeArray.MostDetailedMip = j;
						 shaderResourceViewDesc.TextureCubeArray.MipLevels = 1;

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->additionalSRVs.back());
						 assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
					 }
				 }
				 else
				 {
					 UINT slices = arraysize;

					 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
					 shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
					 shaderResourceViewDesc.Texture2DArray.ArraySize = pTexture->desc.ArraySize;

					 for (UINT j = 0; j < pTexture->desc.MipLevels; ++j)
					 {
						 shaderResourceViewDesc.Texture2DArray.MostDetailedMip = j;
						 shaderResourceViewDesc.Texture2DArray.MipLevels = 1;

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->additionalSRVs.back());
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
						 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
						 shaderResourceViewDesc.TextureCubeArray.First2DArrayFace = i * 6;
						 shaderResourceViewDesc.TextureCubeArray.NumCubes = 1;
						 shaderResourceViewDesc.TextureCubeArray.MostDetailedMip = 0;
						 shaderResourceViewDesc.TextureCubeArray.MipLevels = -1;

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource,&shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->additionalSRVs.back());
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
							 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
							 shaderResourceViewDesc.Texture2DMSArray.FirstArraySlice = i;
							 shaderResourceViewDesc.Texture2DMSArray.ArraySize = 1;
						 }
						 else
						 {
							 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
							 shaderResourceViewDesc.Texture2DArray.FirstArraySlice = i;
							 shaderResourceViewDesc.Texture2DArray.ArraySize = 1;
							 shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
							 shaderResourceViewDesc.Texture2DArray.MipLevels = -1;
						 }

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource,&shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->additionalSRVs.back());
						assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");

					 }
				 }
			 }
		 }
		 else
		 {
			 if (multisampled)
			 {
				 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
				 hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->SRV);
				 assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
			 }
			 else
			 {
				 shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				 if (pTexture->indepentdentSRVMIPs)
				 {
					 UINT miplevels = pTexture->desc.MipLevels;
					 for (UINT i = 0; i < miplevels; ++i)
					 {
						 shaderResourceViewDesc.Texture2D.MostDetailedMip = i;
						 shaderResourceViewDesc.Texture2D.MipLevels = 1;

						 pTexture->additionalSRVs.push_back(NUll_Handle);
						 hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->additionalSRVs.back());
						 assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
					 }
				 }
				 {
					 shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
					 shaderResourceViewDesc.Texture2D.MipLevels = -1;
					 hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->SRV);
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

		D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = ConvertFormat(pTexture->desc.Format);
		shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;

		if (pTexture->indepentdentSRVMIPs)
		{
			UINT miplevels = pTexture->desc.MipLevels;
			for (UINT i = 0; i < miplevels; ++i)
			{
				shaderResourceViewDesc.Texture3D.MostDetailedMip = i;
				shaderResourceViewDesc.Texture3D.MipLevels = 1;

				pTexture->additionalSRVs.push_back(NUll_Handle);
				hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->additionalSRVs[i]);
				assert(SUCCEEDED(hr) && "ShaderResourceView Creation failed!");
			}
		}

		{
			
			shaderResourceViewDesc.Texture3D.MostDetailedMip = 0; 
			shaderResourceViewDesc.Texture3D.MipLevels = -1; 

			hr = device->CreateShaderResourceView((ID3D12Resource*)pTexture->resource, &shaderResourceViewDesc, (ID3D12ShaderResourceView**)&pTexture->SRV);
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

		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = ConvertFormat(pTexture->desc.Format);
		renderTargetViewDesc.Texture2DArray.MipSlice = 0;

		if (pTexture->desc.MiscFlags & RESOURCE_MISC_TEXTURECUBE)
		{
			
			UINT slices = arraySize / 6;

			renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
			renderTargetViewDesc.Texture2DArray.MipSlice = 0;

			if (pTexture->independentRTVCubemapFaces)
			{
				
				for (UINT i = 0; i < arraySize; ++i)
				{
					renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
					renderTargetViewDesc.Texture2DArray.ArraySize = 1;

					pTexture->additionalRTVs.push_back(NUll_Handle);
					hr = device->CreateRenderTargetView((ID3D12Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D12RenderTargetView**)&pTexture->additionalRTVs[i]);
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
					hr = device->CreateRenderTargetView((ID3D12Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D12RenderTargetView**)&pTexture->additionalRTVs[i]);
					assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
				}
			}

			{
				
				renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
				renderTargetViewDesc.Texture2DArray.ArraySize = arraySize;

				hr = device->CreateRenderTargetView((ID3D12Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D12RenderTargetView**)&pTexture->RTV);
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
						renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
						renderTargetViewDesc.Texture2DMSArray.FirstArraySlice = i;
						renderTargetViewDesc.Texture2DMSArray.ArraySize = 1;
					}
					else
					{
						renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
						renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
						renderTargetViewDesc.Texture2DArray.ArraySize = 1;
						renderTargetViewDesc.Texture2DArray.MipSlice = 0;
					}

					pTexture->additionalRTVs.push_back(NUll_Handle);
					hr = device->CreateRenderTargetView((ID3D12Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D12RenderTargetView**)&pTexture->additionalRTVs[i]);
					assert(SUCCEEDED(hr) && "RenderTargetView Creation failed!");
				}
			}

			{
				
				if (arraySize > 1)
				{
					if (multisampled)
					{
						renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
						renderTargetViewDesc.Texture2DMSArray.FirstArraySlice = 0;
						renderTargetViewDesc.Texture2DMSArray.ArraySize = arraySize;
					}
					else
					{
						renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
						renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
						renderTargetViewDesc.Texture2DArray.ArraySize = arraySize;
						renderTargetViewDesc.Texture2DArray.MipSlice = 0;
					}
				}
				else
				{
					if (multisampled)
					{
						renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
					}
					else
					{
						renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
						renderTargetViewDesc.Texture2D.MipSlice = 0;
					}
				}

				hr = device->CreateRenderTargetView((ID3D12Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D12RenderTargetView**)&pTexture->RTV);
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
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = ConvertFormat(pTexture->desc.Format);

		if (pTexture->independentRTVArraySlices)
		{
			for (UINT i=0;i<pTexture->GetDesc().Depth;++i)
			{
				renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
				renderTargetViewDesc.Texture3D.MipSlice = 0;
				renderTargetViewDesc.Texture3D.FirstWSlice = i;
				renderTargetViewDesc.Texture3D.WSize = 1;

				pTexture->additionalRTVs.push_back(NUll_Handle);
				hr = device->CreateRenderTargetView((ID3D12Resource*)pTexture->resource,&renderTargetViewDesc, (ID3D12RenderTargetView**)&pTexture->additionalRTVs[i]);
			}
		}

		{
			renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
			renderTargetViewDesc.Texture3D.MipSlice = 0;
			renderTargetViewDesc.Texture3D.FirstWSlice = 0;
			renderTargetViewDesc.Texture3D.WSize = -1;

			pTexture->additionalRTVs.push_back(NUll_Handle);
			hr = device->CreateRenderTargetView((ID3D12Resource*)pTexture->resource, &renderTargetViewDesc, (ID3D12RenderTargetView**)&pTexture->additionalRTVs[0]);
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

		D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
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

			depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
			depthStencilViewDesc.Texture2DArray.MipSlice = 0;

			if (pTexture->independentRTVCubemapFaces)
			{
				for (UINT i=0;i<arraysize;++i)
				{
					depthStencilViewDesc.Texture2DArray.FirstArraySlice = i;
					depthStencilViewDesc.Texture2DArray.ArraySize = 1;

					pTexture->additionalDSVs.push_back(NUll_Handle);
					hr = device->CreateDepthStencilView((ID3D12Resource*)pTexture->resource, &depthStencilViewDesc, (ID3D12DepthStencilView**)&pTexture->additionalDSVs[i]);
				}
			}
			else if(pTexture->independentRTVArraySlices)
			{
				for (UINT i=0;i<slices;++i)
				{
					depthStencilViewDesc.Texture2DArray.FirstArraySlice = i * 6;
					depthStencilViewDesc.Texture2DArray.ArraySize = 6;

					pTexture->additionalDSVs.push_back(NUll_Handle);
					hr = device->CreateDepthStencilView((ID3D12Resource*)pTexture->resource,&depthStencilViewDesc, (ID3D12DepthStencilView**)&pTexture->additionalDSVs[i]);
				}
			}
			{
				depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
				depthStencilViewDesc.Texture2DArray.ArraySize = arraysize;
			
				hr = device->CreateDepthStencilView((ID3D12Resource*)pTexture->resource,&depthStencilViewDesc, (ID3D12DepthStencilView**)&pTexture->DSV);
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
						depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
						depthStencilViewDesc.Texture2DMSArray.FirstArraySlice = i;
						depthStencilViewDesc.Texture2DMSArray.ArraySize = 1;
					}
					else
					{
						depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
						depthStencilViewDesc.Texture2DArray.MipSlice = 0;
						depthStencilViewDesc.Texture2DArray.FirstArraySlice = i;
						depthStencilViewDesc.Texture1DArray.ArraySize = 1;
					}

					pTexture->additionalDSVs.push_back(NUll_Handle);
					hr = device->CreateDepthStencilView((ID3D12Resource*)pTexture->resource, &depthStencilViewDesc, (ID3D12DepthStencilView**)&pTexture->additionalDSVs[i]);
				}
			}
			else
			{
				if (arraysize>1)
				{
					if (multisample)
					{
						depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
						depthStencilViewDesc.Texture2DMSArray.FirstArraySlice = 0;
						depthStencilViewDesc.Texture2DMSArray.ArraySize = arraysize;
					}
					else
					{
						depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
						depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
						depthStencilViewDesc.Texture2DArray.ArraySize = arraysize;
						depthStencilViewDesc.Texture2DArray.MipSlice = 0;
					}
				}
				else
				{
					if (multisample)
					{
						depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
					}
					else
					{
						depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
						depthStencilViewDesc.Texture2D.MipSlice = 0;
					}
				}
				hr = device->CreateDepthStencilView((ID3D12Resource*)pTexture->resource,&depthStencilViewDesc, (ID3D12DepthStencilView**)&pTexture->DSV);
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

	D3D12_INPUT_ELEMENT_DESC* desc = new D3D12_INPUT_ELEMENT_DESC[NumElements];
	for (UINT i = 0; i < NumElements; ++i)
	{
		desc[i].SemanticName = pInputElementDescs[i].SemanticName;
		desc[i].SemanticIndex = pInputElementDescs[i].SemanticIndex;
		desc[i].Format = ConvertFormat(pInputElementDescs[i].Format);
		desc[i].InputSlot = pInputElementDescs[i].InputSlot;
		desc[i].AlignedByteOffset = pInputElementDescs[i].AlignedByteOffset;
		if (desc[i].AlignedByteOffset == APPEND_ALIGNED_ELEMENT)
			desc[i].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		desc[i].InputSlotClass = ConvertInputClassification(pInputElementDescs[i].InputSlotClass);
		desc[i].InstanceDataStepRate = pInputElementDescs[i].InstanceDataStepRate;

		pInputLayout->desc.push_back(pInputElementDescs[i]);
	}

	HRESULT hr = device->CreateInputLayout(desc, NumElements, ((ID3D10Blob*)(blob->resource))->GetBufferPointer(), ((ID3D10Blob*)(blob->resource))->GetBufferSize(), (ID3D12InputLayout**)&pInputLayout->resource);

	SAFE_DELETE_ARRAY(desc);

	return hr;
}

HRESULT RenderDevice_DX11::CreateVertexShader(WCHAR* filename, GraphicBlob* blob, VertexShader* pVertexShader)
{

	pVertexShader->Register(this);
	ID3D10Blob* ERRORMESSAGE;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_DEBUG|D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resource, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		assert("vs create fail");
	}
	return device->CreateVertexShader(((ID3D10Blob*)(blob->resource))->GetBufferPointer(), ((ID3D10Blob*)(blob->resource))->GetBufferSize(), nullptr, (ID3D12VertexShader**)&pVertexShader->resource);
}

HRESULT RenderDevice_DX11::CreatePixelShader(WCHAR* filename, GraphicBlob* blob, PixelShader* pPixelShader)
{
	pPixelShader->Register(this);
	ID3D10Blob* ERRORMESSAGE=NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resource, &ERRORMESSAGE);
	if (ERRORMESSAGE!=nullptr)
	{
		OutputDebugStringA((char*)ERRORMESSAGE->GetBufferPointer());
	}
	
	if (FAILED(hr))
	{
		assert("ps create fail");
	}
	return device->CreatePixelShader(((ID3D10Blob*)(blob->resource))->GetBufferPointer(), ((ID3D10Blob*)(blob->resource))->GetBufferSize(), nullptr, (ID3D12PixelShader**)&pPixelShader->resource);
}

HRESULT RenderDevice_DX11::CreateGemotryShader(WCHAR* filename, GraphicBlob* blob, GeometryShader* pGeometryShader)
{
	pGeometryShader->Register(this);
	ID3D10Blob* ERRORMESSAGE = NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "gs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resource, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		OutputDebugStringA("gs create fail");
	}
	return device->CreateGeometryShader(((ID3D10Blob*)(blob->resource))->GetBufferPointer(), ((ID3D10Blob*)(blob->resource))->GetBufferSize(), nullptr, (ID3D12GeometryShader**)&pGeometryShader->resource);
}

HRESULT RenderDevice_DX11::CreateHullShader(WCHAR* filename, GraphicBlob* blob, HullShader* pHullShader)
{
	pHullShader->Register(this);
	ID3D10Blob* ERRORMESSAGE = NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "hs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resource, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		assert("hs create fail");
	}
	return device->CreateHullShader(((ID3D10Blob*)(blob->resource))->GetBufferPointer(), ((ID3D10Blob*)(blob->resource))->GetBufferSize(), nullptr, (ID3D12HullShader**)&pHullShader->resource);
}

HRESULT RenderDevice_DX11::CreateDomainShader(WCHAR* filename, GraphicBlob* blob, DomainShader* pDomainShader)
{
	pDomainShader->Register(this);
	ID3D10Blob* ERRORMESSAGE = NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, NULL, "main", "ds_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resource, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		assert("ds create fail");
	}
	return device->CreateDomainShader(((ID3D10Blob*)(blob->resource))->GetBufferPointer(), ((ID3D10Blob*)(blob->resource))->GetBufferSize(), nullptr, (ID3D12DomainShader**)&pDomainShader->resource);
}
HRESULT RenderDevice_DX11::CreateComputerShader(WCHAR* filename, GraphicBlob* blob, ComputerShader* pComputerShader)
{
	pComputerShader->Register(this);
	ID3D10Blob* ERRORMESSAGE = NULL;
	HRESULT hr = D3DCompileFromFile(filename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, NULL, (ID3D10Blob**)&blob->resource, &ERRORMESSAGE);
	if (FAILED(hr))
	{
		assert("cs create fail");
	}
	return device->CreateComputeShader(((ID3D10Blob*)(blob->resource))->GetBufferPointer(), ((ID3D10Blob*)(blob->resource))->GetBufferSize(), nullptr, (ID3D12ComputeShader**)&pComputerShader->resource);
}

HRESULT RenderDevice_DX11::CreateBlendState(const BlendStateDesc *pBlendStateDesc, BlendState *pBlendState)
{
	pBlendState->Register(this);

	D3D12_BLEND_DESC desc;
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
	return device->CreateBlendState(&desc, (ID3D12BlendState**)&pBlendState->resource);

}

HRESULT RenderDevice_DX11::CreateDepthStencilState(const DepthStencilStateDesc* pDepthStencilStateDesc, DepthStencilState* pDepthStencilState)
{
	pDepthStencilState->Register(this);

	D3D12_DEPTH_STENCIL_DESC desc;
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
	return device->CreateDepthStencilState(&desc, (ID3D12DepthStencilState**)&pDepthStencilState->resource);
}

HRESULT RenderDevice_DX11::CreateRasterizerState(const RasterizerStateDesc *pRasterizerStateDesc, RasterizerState *pRasterizerState)
{
	pRasterizerState->Register(this);

	pRasterizerState->desc = *pRasterizerStateDesc;

	D3D12_RASTERIZER_DESC desc;
	desc.FillMode = ConvertFillMode(pRasterizerStateDesc->FillMode);
	desc.CullMode = ConvertCullMode(pRasterizerStateDesc->CullMode);
	desc.FrontCounterClockwise = pRasterizerStateDesc->FrontCounterClockWise;
	desc.DepthBias = pRasterizerStateDesc->DepthBias;
	desc.DepthBiasClamp = pRasterizerStateDesc->DepthBiasClamp;
	desc.SlopeScaledDepthBias = pRasterizerStateDesc->SlopeScaledDepthBias;
	desc.DepthClipEnable = pRasterizerStateDesc->DepthCilpEnable;
	desc.ScissorEnable = pRasterizerStateDesc->ScissorEnable;
	desc.MultisampleEnable = pRasterizerStateDesc->MultisampleEnable;
	desc.AntialiasedLineEnable = pRasterizerStateDesc->AntialiasedLineEnable;

	if (CONSERVATIVE_RASTERIZATION && pRasterizerStateDesc->ConservativeRasterizationEnable==TRUE)
	{
		ID3D12Device3* device3 = nullptr;
		if (SUCCEEDED(device->QueryInterface(__uuidof(ID3D12Device),(void**)&device3)))
		{
			D3D12_RASTERIZER_DESC2 desc2;
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

			ID3D12RasterizerState2* rasterizer2 = nullptr;
			HRESULT hr = device3->CreateRasterizerState2(&desc2, &rasterizer2);
			pRasterizerState->resource = (CPUHandle)rasterizer2;
			SAFE_RELEASE(device3);
			return hr;
		}
	}

	else if (RASTERIZER_ORDERED_VIEWS && pRasterizerStateDesc->ForceSampleCount>0)
	{
		ID3D12Device1* device1 = nullptr;
		if (SUCCEEDED(device->QueryInterface(__uuidof(ID3D12Device1),(void**)&device1)))
		{
			D3D12_RASTERIZER_DESC1 desc1;
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

			ID3D12RasterizerState1* rasterizer1 = nullptr;
			HRESULT hr = device1->CreateRasterizerState1(&desc1, &rasterizer1);
			pRasterizerState->resource = (CPUHandle)rasterizer1;
			SAFE_RELEASE(device1);
			return hr;
		}
	}
	return device->CreateRasterizerState(&desc, (ID3D12RasterizerState**)&pRasterizerState->resource);
}

HRESULT RenderDevice_DX11::CreateSamplerState(const SamplerDesc *pSamplerDesc, Sampler *pSamplerState)
{
	pSamplerState->Register(this);

	D3D12_SAMPLER_DESC desc;
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
	return device->CreateSamplerState(&desc, (ID3D12SamplerState**)&pSamplerState->resource);
}

HRESULT RenderDevice_DX11::CreateQuery(const GPUQueryDesc *pDesc, GPUQuery *pQuery)
{
	pQuery->Register(this);

	HRESULT hr = E_FAIL;

	pQuery->desc = *pDesc;
	pQuery->async_frameshift = pQuery->desc.async_Latnce;

	D3D12_QUERY_DESC desc;
	desc.MiscFlags = 0;
	desc.Query = D3D12_QUERY_OCCLUSION_PREDICATE;
	if (pDesc->Type==GPU_QUERY_TYPE_OCCLUSION)
	{
		desc.Query = D3D12_QUERY_OCCLUSION;
	}
	else if (pDesc->Type== GPU_QUERY_TYPE_TIMESTAMP)
	{
		desc.Query = D3D12_QUERY_TIMESTAMP;
	}
	else if (pDesc->Type == GPU_QUERY_TYPE_TIMESTAMP_DISJOINT)
	{
		desc.Query = D3D12_QUERY_TIMESTAMP_DISJOINT;
	}

	if (pQuery->desc.async_Latnce > 0)
	{
		pQuery->resources.resize(pQuery->desc.async_Latnce + 1);
		pQuery->active.resize(pQuery->desc.async_Latnce + 1);
		for (size_t i=0;i<pQuery->resources.size();++i) 
		{
			hr = device->CreateQuery(&desc, (ID3D12Query**)&pQuery->resources[i]);
			assert(SUCCEEDED(hr) && "GPUQuery creation failed!");
		}
	}
	else
	{
		pQuery->resources.resize(1);
		pQuery->active.resize(1);
		hr = device->CreateQuery(&desc, (ID3D12Query**)&pQuery->resources[0]);
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
		((ID3D12Resource*)pResource->resource)->Release();
	}

	if (pResource->SRV != NUll_Handle)
	{
		((ID3D12ShaderResourceView*)pResource->SRV)->Release();
	}
	for (auto& x : pResource->additionalSRVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D12ShaderResourceView*)x)->Release();
		}
	}

	if (pResource->UAV != NUll_Handle)
	{
		((ID3D12UnorderedAccessView*)pResource->UAV)->Release();
	}
	for (auto& x : pResource->additionalUAVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D12UnorderedAccessView*)x)->Release();
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
		((ID3D12RenderTargetView*)pTexture1D->RTV)->Release();
	}
	for (auto& x : pTexture1D->additionalRTVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D12RenderTargetView*)x)->Release();
		}
	}
}

void RenderDevice_DX11::DestroyTexture2D(Texture2D* pTexture2D)
{
	if (pTexture2D->RTV != NUll_Handle)
	{
		((ID3D12RenderTargetView*)pTexture2D->RTV)->Release();
	}
	for (auto& x : pTexture2D->additionalRTVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D12RenderTargetView*)x)->Release();
		}
	}

	if (pTexture2D->DSV != NUll_Handle)
	{
		((ID3D12DepthStencilView*)pTexture2D->DSV)->Release();
	}
	for (auto& x : pTexture2D->additionalDSVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D12DepthStencilView*)x)->Release();
		}
	}

}

void RenderDevice_DX11::DestroyTexture3D(Texture3D* pTexture3D)
{
	if (pTexture3D->RTV != NUll_Handle)
	{
		((ID3D12RenderTargetView*)pTexture3D->RTV)->Release();
	}
	for (auto& x : pTexture3D->additionalRTVs)
	{
		if (x != NUll_Handle)
		{
			((ID3D12RenderTargetView*)x)->Release();
		}
	}
}

void RenderDevice_DX11::DestroyInputLayout(VertexLayout *pInputLayout)
{
	if (pInputLayout->resource != NUll_Handle)
	{
		((ID3D12InputLayout*)pInputLayout->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyVertexShader(VertexShader* pVertexShader)
{
	if (pVertexShader->resource != NUll_Handle)
	{
		((ID3D12VertexShader*)pVertexShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyPixelShader(PixelShader* pPixelShader)
{

	if (pPixelShader->resource != NUll_Handle)
	{
		((ID3D12PixelShader*)pPixelShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyGeometryShader(GeometryShader* pGeometryShader)
{
	if (pGeometryShader->resource != NUll_Handle)
	{
		((ID3D12GeometryShader*)pGeometryShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyHullShader(HullShader* pHullShader)
{
	if (pHullShader->resource != NUll_Handle)
	{
		((ID3D12HullShader*)pHullShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyDomainShader(DomainShader* pDomainShader)
{
	if (pDomainShader->resource != NUll_Handle)
	{
		((ID3D12DomainShader*)pDomainShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyComputerShader(ComputerShader* pComputerShader)
{
	if (pComputerShader->resource != NUll_Handle)
	{
		((ID3D12ComputeShader*)pComputerShader->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyBlendState(BlendState* pBlendState)
{
	if (pBlendState->resource != NUll_Handle)
	{
		((ID3D12BlendState*)pBlendState->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyDepthStencilState(DepthStencilState* pDepthStencilState)
{
	if (pDepthStencilState->resource != NUll_Handle)
	{
		((ID3D12DepthStencilState*)pDepthStencilState->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyRasterizerState(RasterizerState* pRasterizerstate)
{
	if (pRasterizerstate->resource != NUll_Handle)
	{
		((ID3D12RasterizerState*)pRasterizerstate->resource)->Release();
	}
}

void RenderDevice_DX11::DestroySamplerState(Sampler* pSamplerState)
{
	if (pSamplerState->resource != NUll_Handle)
	{
		((ID3D12SamplerState*)pSamplerState->resource)->Release();
	}
}

void RenderDevice_DX11::DestroyQuery(GPUQuery *pQuey)
{
	for (auto& x : pQuey->resources)
	{
		if (x != NUll_Handle)
		{
			((ID3D12Query*)x)->Release();
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
	((ID3D12Resource*)pResource->resource)->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
}

void RenderDevice_DX11::SetName(RenderDeviceChild * pResource, const std::string & name)
{
	((ID3D12DeviceChild*)pResource->resource)->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
}

void RenderDevice_DX11::PresentBegin()
{
		const float color[4] = { 0.f, 0.0f, 0.000000000f, 1.000000000f };
		ImmediatedeviceContext->ClearRenderTargetView(renderTargetView, color);
		ImmediatedeviceContext->ClearDepthStencilView(DepthStecilView, D3D12_CLEAR_DEPTH | D3D12_CLEAR_STENCIL, 1.0f, 0);
}


void RenderDevice_DX11::PresentEnd()
{
	DXGI_PRESENT_PARAMETERS parameters = { 0 };
	swapChain->Present1(VSYNC, 0,&parameters);


	ImmediatedeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	ImmediatedeviceContext->ClearState();
	BindGraphicsPSO(nullptr);
	BindComputerPSO(nullptr);

	D3D12_RECT pRects[8];
	for (UINT i=0;i<8;++i)
	{
		pRects[i].bottom = INT32_MAX;
		pRects[i].left = INT32_MIN;
		pRects[i].right = INT32_MAX;
		pRects[i].top = INT32_MIN;
	}
	ImmediatedeviceContext->RSSetScissorRects(0, pRects);


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
	memset(raster_uavs_count, 0, sizeof(raster_uavs_count));

	FRAMECOUNT++;


	RESOLUTIONCHANGED = false;
}

void RenderDevice_DX11::ExcuteDeferredContexts()
{
		for (int i=0;i<9;i++)
		{
			if (commandlists[i]!=nullptr)
			{
				ImmediatedeviceContext->ExecuteCommandList(commandlists[i], false);
				commandlists[i]->Release();
				commandlists[i] = nullptr;
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

	hr = swapChain->GetBuffer(0, __uuidof(ID3D12Texture2D), (LPVOID*)&backbuffer);
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
	D3D12_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = SCREENWIDTH;
	depthStencilDesc.Height = SCREENHEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D12_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D12_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	Basedevice->CreateTexture2D(&depthStencilDesc, NULL, &depthbuffer);
	Basedevice->CreateDepthStencilView(depthbuffer, NULL, &DepthStecilView);
}


void RenderDevice_DX11::BindScissorRects(UINT numRects, const Rect* rect)
{
	assert(rect!=nullptr);
	assert(numRects <= 8);
	D3D12_RECT pRects[8];
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
	D3D12_VIEWPORT d3dViewPorts[6];
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
	ID3D12RenderTargetView* rendertargetViews[8];
	ZeroMemory(rendertargetViews, sizeof(rendertargetViews));
	for (UINT i = 0; i < min(NumViews, 8); ++i)
	{
		if (arrayIndex < 0 || ppRenderTargets[i]->additionalRTVs.empty())
		{
			rendertargetViews[i] = (ID3D12RenderTargetView*)ppRenderTargets[i]->RTV;
		}
		else
		{
			assert(ppRenderTargets[i]->additionalRTVs.size() > static_cast<size_t>(arrayIndex) && "Invaild");
			rendertargetViews[i] = (ID3D12RenderTargetView*)ppRenderTargets[i]->additionalRTVs[arrayIndex];
		}
	}

	ID3D12DepthStencilView* depthStencilView = nullptr;
	if (depthStencilTexture !=nullptr)
	{
		if (arrayIndex<0||depthStencilTexture->additionalDSVs.empty())
		{
			depthStencilView = (ID3D12DepthStencilView*)depthStencilTexture->DSV;
		}
		else
		{
			assert(depthStencilTexture->additionalDSVs.size() > static_cast<size_t>(arrayIndex) && "Ivalide");
			depthStencilView = (ID3D12DepthStencilView*)depthStencilTexture->additionalDSVs[arrayIndex];
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

void RenderDevice_DX11::BindBackBufferRenderTargets(Texture2D* depthStencilTexture)
{
	ID3D12DepthStencilView* depthStencilView = nullptr;
	if (depthStencilView != nullptr)
	{
			depthStencilView = (ID3D12DepthStencilView*)depthStencilTexture->DSV;
	}
	for (int i = 0; i < JobScheduler::NumThreads; i++)
	{
		deviceContexts[i]->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	}
}



void RenderDevice_DX11::ClearRenderTarget(Texture* pTexture, const FLOAT ColorRGBA[4],int arrayIndex /*= -1*/)
{
	if (arrayIndex<0)
	{
		deviceContexts[ThreadID]->ClearRenderTargetView((ID3D12RenderTargetView*)pTexture->RTV, ColorRGBA);
	}
	else
	{
		assert(pTexture->additionalRTVs.size() > static_cast<size_t>(arrayIndex) && "Invalid");
		deviceContexts[ThreadID]->ClearRenderTargetView((ID3D12RenderTargetView*)pTexture->additionalRTVs[arrayIndex], ColorRGBA);
	}
}

void RenderDevice_DX11::ClearDepthStencil(Texture2D* pTexture, UINT ClearFlags, FLOAT Depth, UINT8 Stencil, int arrayIndex /*= -1*/)
{
	UINT flag = 0;
	if (ClearFlags&CLEAR_DEPTH)
	{
		flag |= D3D12_CLEAR_DEPTH;
	}
	if (ClearFlags&CLEAR_STENCIL)
	{
		flag |= D3D12_CLEAR_STENCIL;
	}

	if (arrayIndex<0)
	{
		deviceContexts[ThreadID]->ClearDepthStencilView((ID3D12DepthStencilView*)pTexture->DSV, flag, Depth, Stencil);
	}
	else
	{
		assert(pTexture->additionalDSVs.size() > static_cast<size_t>(arrayIndex) && "Invalid");
		deviceContexts[ThreadID]->ClearDepthStencilView((ID3D12DepthStencilView*)pTexture->additionalDSVs[arrayIndex], flag, Depth, Stencil);
	}
}

void RenderDevice_DX11::BindResource(SHADERSTAGE stage, GPUResource* resource, int slot, int arrayIndex /*= -1*/)
{
	if (resource != nullptr)
	{
		ID3D12ShaderResourceView* SRV;

		if (arrayIndex < 0)
		{
			SRV = (ID3D12ShaderResourceView*)resource->SRV;
		}
		else
		{
			assert(resource->additionalSRVs.size() > static_cast<size_t>(arrayIndex) && "Invalid arrayIndex!");
			SRV = (ID3D12ShaderResourceView*)resource->additionalSRVs[arrayIndex];
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
	ID3D12ShaderResourceView* srvs[8];
	for (int i = 0; i < count; ++i)
	{
		srvs[i] = resources[i] != nullptr ? (ID3D12ShaderResourceView*)resources[i]->SRV : nullptr;
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
		ID3D12UnorderedAccessView* UAV = (ID3D12UnorderedAccessView*)resources->UAV;

		if (arrayIndex < 0)
		{
			UAV = (ID3D12UnorderedAccessView*)resources->UAV;
		}
		else
		{
			assert(resources->additionalUAVs.size() > static_cast<size_t>(arrayIndex) && "Invalid arrayIndex!");
			UAV = (ID3D12UnorderedAccessView*)resources->additionalUAVs[arrayIndex];
		}

		if (stage == CS_STAGE)
		{
			deviceContexts[ThreadID]->CSSetUnorderedAccessViews(slot, 1, &UAV, nullptr);
		}
		else
		{
			raster_uavs[ThreadID][slot] = (ID3D12UnorderedAccessView*)resources->UAV;
			raster_uavs_slot[ThreadID] = min(raster_uavs_slot[ThreadID], slot);
			raster_uavs_count[ThreadID] = max(raster_uavs_count[ThreadID], 1);
		}
	}
}

void RenderDevice_DX11::BindUAVS(SHADERSTAGE stage, GPUResource* const* resource, int slot, int count)
{
	assert(slot + count <= 8);
	ID3D12UnorderedAccessView* uavs[8];
	for (int i=0;i<count;++i)
	{
		uavs[i] = resource[i] != nullptr ? (ID3D12UnorderedAccessView*)resource[i]->UAV : nullptr;

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
	deviceContexts[ThreadID]->PSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->VSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->GSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->HSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->DSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	deviceContexts[ThreadID]->CSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
}

void RenderDevice_DX11::UnbindUAVs(int slot, int num)
{
	assert(num <= ARRAYSIZE(nullBlob) && "Extend nullBlob to support more resource unbinding");
	deviceContexts[ThreadID]->CSSetUnorderedAccessViews(slot, num, (ID3D12UnorderedAccessView**)nullBlob, 0);

	raster_uavs_count[ThreadID] = 0;
	raster_uavs_slot[ThreadID] = 8;
}

void RenderDevice_DX11::BindSampler(SHADERSTAGE stage, Sampler* sampler, int slot,int count)
{

	ID3D12SamplerState* SAMPLER = (ID3D12SamplerState*)sampler->resource;
	switch (stage)
	{
	case PRE::VS_STAGE:
		deviceContexts[ThreadID]->VSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::HS_STAGE:
		deviceContexts[ThreadID]->HSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::DS_STAGE:
		deviceContexts[ThreadID]->DSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::GS_STAGE:
		deviceContexts[ThreadID]->GSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::PS_STAGE:
		deviceContexts[ThreadID]->PSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::CS_STAGE:
		deviceContexts[ThreadID]->CSSetSamplers(slot, count, &SAMPLER);
		break;
	default:
		assert(0);
		break;
	}
}

void RenderDevice_DX11::BindConstantBuffer(SHADERSTAGE stage, GPUBuffer* buffer, int slot, const UINT *pFirstConstant, const UINT *pNumberConstant)
{
	ID3D12Buffer* res = buffer ? (ID3D12Buffer*)buffer->resource : nullptr;
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
	ID3D12Buffer* res[8] = { 0 };
	for (int i=0;i<count;++i)
	{
		res[i] = vertexBuffers[i] != nullptr ? (ID3D12Buffer*)vertexBuffers[i]->resource : nullptr;
	}
	deviceContexts[ThreadID]->IASetVertexBuffers(static_cast<UINT>(slot), static_cast<UINT>(count),res,strides, (offsets != nullptr ? offsets : reinterpret_cast<const UINT*>(nullBlob)));
}

void RenderDevice_DX11::BindIndexBuffer(GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, UINT offset)
{
	ID3D12Buffer* res = indexBuffer != nullptr ? (ID3D12Buffer*)indexBuffer->resource : nullptr;
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

	ID3D12VertexShader* vs = desc.vs == nullptr ? nullptr : (ID3D12VertexShader*)desc.vs->resource;
	if (vs !=nullptr)
	{
		deviceContexts[ThreadID]->VSSetShader(vs, nullptr, 0);
	}
	ID3D12PixelShader* ps = desc.ps == nullptr ? nullptr : (ID3D12PixelShader*)desc.ps->resource;
	if (ps != nullptr)
	{
		deviceContexts[ThreadID]->PSSetShader(ps, nullptr, 0);
	}
	ID3D12HullShader* hs = desc.hs == nullptr ? nullptr : (ID3D12HullShader*)desc.hs->resource;
	if (hs != nullptr)
	{
		deviceContexts[ThreadID]->HSSetShader(hs, nullptr, 0);
	}
	ID3D12DomainShader* ds = desc.ds == nullptr ? nullptr : (ID3D12DomainShader*)desc.ds->resource;
	if (ds != nullptr)
	{
		deviceContexts[ThreadID]->DSSetShader(ds, nullptr, 0);
	}
	ID3D12GeometryShader* gs = desc.gs == nullptr ? nullptr : (ID3D12GeometryShader*)desc.gs->resource;
	if (gs != nullptr)
	{
		deviceContexts[ThreadID]->GSSetShader(gs, nullptr, 0);
	}

	ID3D12BlendState* bs = desc.bs == nullptr ? nullptr : (ID3D12BlendState*)desc.bs->resource;
	if (bs != nullptr )
	{
		const float fact[4] = { blendFactor[0].x, blendFactor[0].y, blendFactor[0].z, blendFactor[0].w };
		deviceContexts[ThreadID]->OMSetBlendState(bs, fact, desc.sampleMask);
	}

	ID3D12RasterizerState* rs = desc.rs == nullptr ? nullptr : (ID3D12RasterizerState*)desc.rs->resource;
	if (rs != nullptr)
	{
		deviceContexts[ThreadID]->RSSetState(rs);
	}

	ID3D12DepthStencilState* dss = desc.dss == nullptr ? nullptr : (ID3D12DepthStencilState*)desc.dss->resource;
	if (dss != nullptr)
	{
		deviceContexts[ThreadID]->OMSetDepthStencilState(dss, stencilRef[0]);
	}

	ID3D12InputLayout* il = desc.VL == nullptr ? nullptr : (ID3D12InputLayout*)desc.VL->resource;
	if (il != nullptr)
	{
		deviceContexts[ThreadID]->IASetInputLayout(il);
	}

	{
		D3D12_PRIMITIVE_TOPOLOGY d3dType = D3D12_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		switch (desc.PT)
		{
		case TRIANGELIST:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		case TRIANGESTRIP:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;
		case POINTLIST:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		case LINELIST:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case PATCHLIST:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
			break;
		default:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_UNDEFINED;
			break;
		};
		deviceContexts[ThreadID]->IASetPrimitiveTopology(d3dType);
	}
}

void RenderDevice_DX11::BindRasterizerState(RasterizerState RSstate)
{
	deviceContexts[ThreadID]->RSSetState((ID3D12RasterizerState*)RSstate.resource);
}

void RenderDevice_DX11::BindComputerPSO(ComputerPSO* pso)
{
	const ComputerPSODesc& desc = pso != nullptr ? pso->GetDesc() : ComputerPSODesc();

	static ID3D12ComputeShader*prev_cs[GRAPHICSTHREAD_COUNT] = {};
	ID3D12ComputeShader* cs = desc.cs == NUll_Handle ? NUll_Handle : (ID3D12ComputeShader*)desc.cs->resource;
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

void RenderDevice_DX11::DrawIndexed(UINT Indexcount, UINT startVertexLocation, UINT baseVertexLocation)
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

	deviceContexts[ThreadID]->DrawInstancedIndirect((ID3D12Buffer*)args->resource,args_offset);
}

void RenderDevice_DX11::Dispatch(UINT threadGruopCountX, UINT threadGruopCountY, UINT threadGruopCountZ)
{
	deviceContexts[ThreadID]->Dispatch(threadGruopCountX, threadGruopCountY, threadGruopCountZ);
}

void RenderDevice_DX11::DispatchIndirect(GPUBuffer* args, UINT args_offset)
{
	deviceContexts[ThreadID]->DispatchIndirect((ID3D12Buffer*)args, args_offset);
}

void RenderDevice_DX11::CopyTexture2D(Texture2D* pDest, Texture2D* pSrc)
{
	deviceContexts[ThreadID]->CopyResource((ID3D12Resource*)pDest->resource, (ID3D12Resource*)pSrc->resource);
}

void RenderDevice_DX11::CopyTexture2D_Region(Texture2D* pDest, UINT dstMip, UINT dstX, UINT dstY, Texture2D* pSrc, UINT srcMip, UINT ArraySize)
{
	deviceContexts[ThreadID]->CopySubresourceRegion((ID3D12Resource*)pDest->resource, D3D12CalcSubresource(dstMip, 0, pDest->GetDesc().MipLevels), dstX, dstY, 0,
		(ID3D12Resource*)pSrc->resource, D3D12CalcSubresource(srcMip, ArraySize, pSrc->GetDesc().MipLevels), nullptr);
}

void RenderDevice_DX11::MSAAResolve(Texture2D* pDst, Texture2D* pSrc)
{
	assert(pDst != nullptr && pSrc != nullptr);
	deviceContexts[ThreadID]->ResolveSubresource((ID3D12Resource*)pDst->resource, 0, (ID3D12Resource*)pSrc->resource, 0, ConvertFormat(pDst->desc.Format));
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
		D3D12_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = deviceContexts[ThreadID]->Map((ID3D12Resource*)buffer->resource,0,D3D12_MAP_WRITE_DISCARD,0,&mappedResource);
		assert(SUCCEEDED(hr) && "GPUBuffer mapping failed!");
		memcpy(mappedResource.pData, data, (datasize >= 0? datasize : buffer->desc.ByteWidth));
		deviceContexts[ThreadID]->Unmap((ID3D12Resource*)buffer->resource, 0);
	}
	else if (buffer->desc.BindFlags & BIND_CONSTANT_BUFFER || datasize < 0)
	{
		deviceContexts[ThreadID]->UpdateSubresource((ID3D12Resource*)buffer->resource, 0, nullptr, data, 0, 0);
	}
	else
	{
		D3D12_BOX box = {};
		box.left = 0;
		box.back = 1;
		box.bottom = 1;
		box.front = 1;
		box.right = static_cast<UINT>(datasize);
		box.top = 0;
		deviceContexts[ThreadID]->UpdateSubresource((ID3D12Resource*)buffer->resource, 0, &box, data, 0, 0);
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

	D3D12_MAP maping = wrap ? D3D12_MAP_WRITE_DISCARD : D3D12_MAP_WRITE_NO_OVERWRITE;
	D3D12_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = deviceContexts[ThreadID]->Map((ID3D12Resource*)buffer->resource, 0, maping, 0, &mappedResource);
	assert(SUCCEEDED(hr) && "GPUBUFFER mapping failed!");

	buffer->byteOffset = position + dataSize;
	buffer->residentFrame = FRAMECOUNT;

	offsetIntoBuffer = (UINT)position;
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(mappedResource.pData) + position);

}

void RenderDevice_DX11::InvalidateBufferAccess(GPUBuffer* buffer)
{
	deviceContexts[ThreadID]->Unmap((ID3D12Resource*)buffer->resource, 0);
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

			deviceContexts[ThreadID]->CopyResource((ID3D12Resource*)bufferDest->resource, (ID3D12Resource*)bufferToDownload->resource);

			D3D12_MAPPED_SUBRESOURCE MappedSubresource = {};
			HRESULT hr = deviceContexts[ThreadID]->Map((ID3D12Resource*)bufferDest->resource, 0, D3D12_MAP_READ,/*asnyc* D3D12_MAP_FLAG_DO_NOT_WAIT*/ 0, &MappedSubresource);
			bool result = SUCCEEDED(hr);
			if (result)
			{
				memcpy(dataDest, MappedSubresource.pData, bufferToDownload->desc.ByteWidth);
				deviceContexts[ThreadID]->Unmap((ID3D12Resource*)bufferDest->resource,0);
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

			deviceContexts[ThreadID]->CopyResource((ID3D12Resource*)TextureDest->resource, (ID3D12Resource*)textureToDownload->resource);

			D3D12_MAPPED_SUBRESOURCE mappedResource = {};
			HRESULT hr = deviceContexts[ThreadID]->Map((ID3D12Resource*)TextureDest->resource, 0, D3D12_MAP_READ, 0, &mappedResource);

			bool result = SUCCEEDED(hr);
			if (result)
			{
				UINT cpycount = max(1, textureToDownload->desc.Width)*max(1, textureToDownload->desc.Height)*max(1, textureToDownload->desc.Depth);
				UINT cpystride = GetFormatStride(textureToDownload->desc.Format);
				UINT cpysize = cpycount*cpystride;
				memcpy(dataDest, mappedResource.pData, cpysize);
				deviceContexts[ThreadID]->Unmap((ID3D12Resource*)TextureDest->resource, 0);
			}
			return result;
		}
	}
	return false;
}
void RenderDevice_DX11::WaitForGPU()
{
	deviceContexts[ThreadID]->Flush();
}

void RenderDevice_DX11::QueryBegin(GPUQuery *query)
{
	deviceContexts[ThreadID]->Begin((ID3D12Query*)query->resources[query->async_frameshift]);
	query->active[query->async_frameshift] = true;
}

void RenderDevice_DX11::QueryEnd(GPUQuery* query)
{
	deviceContexts[ThreadID]->End((ID3D12Query*)query->resources[query->async_frameshift]);
	query->active[query->async_frameshift] = true;
}

bool RenderDevice_DX11::QueryRead(GPUQuery* query)
{
	query->async_frameshift = (query->async_frameshift + 1) % (query->desc.async_Latnce + 1);
	const int readQueryID = query->async_frameshift;
	const UINT flag = (query->desc.async_Latnce > 0 ? D3D12_ASYNC_GETDATA_DONOTFLUSH : 0);

	if (!query->active[readQueryID])
	{
		return true;
	}

	assert(ThreadID == GRAPHICTHREAD_IMMERIATE && "A query can only be read on the immediate graphic thread!");

	HRESULT hr = S_OK;
	switch (query->desc.Type)
	{
	case PRE::GPU_QUERY_TYPE_TIMESTAMP:
		hr = deviceContexts[ThreadID]->GetData((ID3D12Query*)query->resources[readQueryID], &query->result_timestamp, sizeof(query->result_timestamp), flag);
		break;
	case  GPU_QUERY_TYPE_TIMESTAMP_DISJOINT:
		{
			D3D12_QUERY_DATA_TIMESTAMP_DISJOINT temp;
			hr = deviceContexts[ThreadID]->GetData((ID3D12Query*)query->resources[readQueryID], &temp, sizeof(temp), flag);
			query->result_disjoint = temp.Disjoint;
			query->result_timestamp = temp.Frequency;
		}
		break;
	case GPU_QUERY_TYPE_OCCLUSION:
		hr = deviceContexts[ThreadID]->GetData((ID3D12Query*)query->resources[readQueryID], &query->result_passed_sample_count, sizeof(query->result_passed_sample_count), flag);
		break;
	case GPU_QUERY_TYPE_OCCLUSION_PREDICATE:
	default:
		hr = deviceContexts[ThreadID]->GetData((ID3D12Query*)query->resources[readQueryID], &query->result_passed, sizeof(query->result_passed), flag);
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

void* RenderDevice_DX11::GetDevice()
{
	return device;
}

void RenderDevice_DX11::GenerateMips(GPUResource* ShaderResourceView)
{
	deviceContexts[ThreadID]->GenerateMips((ID3D12ShaderResourceView*)ShaderResourceView->SRV);
}

void RenderDevice_DX11::BindScissorRects_Immediate(UINT numRects, const Rect* rect)
{
	assert(rect != nullptr);
	assert(numRects <= 8);
	D3D12_RECT pRects[8];
	for (UINT i = 0; i < numRects; ++i)
	{
		pRects[i].bottom = rect[i].bottom;
		pRects[i].left = rect[i].left;
		pRects[i].right = rect[i].right;
		pRects[i].top = rect[i].top;
	}
	ImmediatedeviceContext->RSSetScissorRects(numRects, pRects);
}

void RenderDevice_DX11::BindViewports_Immediate(UINT NumViewports, const ViewPort* pViewports)
{
	assert(NumViewports <= 6);
	D3D12_VIEWPORT d3dViewPorts[6];
	for (UINT i = 0; i < NumViewports; ++i)
	{
		d3dViewPorts[i].TopLeftX = pViewports[i].TopLeftX;
		d3dViewPorts[i].TopLeftY = pViewports[i].TopLeftY;
		d3dViewPorts[i].Width = pViewports[i].Width;
		d3dViewPorts[i].Height = pViewports[i].Height;
		d3dViewPorts[i].MinDepth = pViewports[i].MinDepth;
		d3dViewPorts[i].MaxDepth = pViewports[i].MaxDepth;
	}

	ImmediatedeviceContext->RSSetViewports(NumViewports, d3dViewPorts);
}

void RenderDevice_DX11::BindRenderTargets_Immediate(UINT NumViews, Texture2D* const *ppRenderTargets, Texture2D* depthStencilTexture, int arrayIndex/* = -1*/)
{
	ID3D12RenderTargetView* rendertargetViews[8];
	ZeroMemory(rendertargetViews, sizeof(rendertargetViews));
	for (UINT i = 0; i < min(NumViews, 8); ++i)
	{
		if (arrayIndex < 0 || ppRenderTargets[i]->additionalRTVs.empty())
		{
			rendertargetViews[i] = (ID3D12RenderTargetView*)ppRenderTargets[i]->RTV;
		}
		else
		{
			assert(ppRenderTargets[i]->additionalRTVs.size() > static_cast<size_t>(arrayIndex) && "Invaild");
			rendertargetViews[i] = (ID3D12RenderTargetView*)ppRenderTargets[i]->additionalRTVs[arrayIndex];
		}
	}

	ID3D12DepthStencilView* depthStencilView = nullptr;
	if (depthStencilView != nullptr)
	{
		if (arrayIndex < 0 || depthStencilTexture->additionalDSVs.empty())
		{
			depthStencilView = (ID3D12DepthStencilView*)depthStencilTexture->DSV;
		}
		else
		{
			assert(depthStencilTexture->additionalDSVs.size() > static_cast<size_t>(arrayIndex) && "Ivalide");
			depthStencilView = (ID3D12DepthStencilView*)depthStencilTexture->additionalDSVs[arrayIndex];
		}
	}

	if (raster_uavs_count[GRAPHICTHREAD_IMMERIATE] > 0)
	{
		const UINT count = raster_uavs_count[GRAPHICTHREAD_IMMERIATE];
		const UINT slot = raster_uavs_slot[GRAPHICTHREAD_IMMERIATE];

		deviceContexts[GRAPHICTHREAD_IMMERIATE]->OMSetRenderTargetsAndUnorderedAccessViews(NumViews, rendertargetViews, depthStencilView, slot, count, &raster_uavs[ThreadID][slot], nullptr);

		raster_uavs_count[GRAPHICTHREAD_IMMERIATE] = 0;
		raster_uavs_slot[GRAPHICTHREAD_IMMERIATE] = 8;
	}
	else
	{
		ImmediatedeviceContext->OMSetRenderTargets(NumViews, rendertargetViews, depthStencilView);
	}

}



void RenderDevice_DX11::ClearRenderTarget_Immediate(Texture* pTexture, const FLOAT ColorRGBA[4], int arrayIndex /*= -1*/)
{
	if (arrayIndex < 0)
	{
		ImmediatedeviceContext->ClearRenderTargetView((ID3D12RenderTargetView*)pTexture->RTV, ColorRGBA);
	}
	else
	{
		assert(pTexture->additionalRTVs.size() > static_cast<size_t>(arrayIndex) && "Invalid");
		ImmediatedeviceContext->ClearRenderTargetView((ID3D12RenderTargetView*)pTexture->additionalRTVs[arrayIndex], ColorRGBA);
	}
}

void RenderDevice_DX11::ClearDepthStencil_Immediate(Texture2D* pTexture, UINT ClearFlags, FLOAT Depth, UINT8 Stencil, int arrayIndex /*= -1*/)
{
	UINT flag = 0;
	if (ClearFlags&CLEAR_DEPTH)
	{
		flag |= D3D12_CLEAR_DEPTH;
	}
	if (ClearFlags&CLEAR_STENCIL)
	{
		flag |= D3D12_CLEAR_STENCIL;
	}

	if (arrayIndex < 0)
	{
		ImmediatedeviceContext->ClearDepthStencilView((ID3D12DepthStencilView*)pTexture->DSV, flag, Depth, Stencil);
	}
	else
	{
		assert(pTexture->additionalDSVs.size() > static_cast<size_t>(arrayIndex) && "Invalid");
		ImmediatedeviceContext->ClearDepthStencilView((ID3D12DepthStencilView*)pTexture->additionalDSVs[arrayIndex], flag, Depth, Stencil);
	}
}

void RenderDevice_DX11::BindResource_Immediate(SHADERSTAGE stage, GPUResource* resource, int slot, int arrayIndex /*= -1*/)
{
	if (resource != nullptr)
	{
		ID3D12ShaderResourceView* SRV;

		if (arrayIndex < 0)
		{
			SRV = (ID3D12ShaderResourceView*)resource->SRV;
		}
		else
		{
			assert(resource->additionalSRVs.size() > static_cast<size_t>(arrayIndex) && "Invalid arrayIndex!");
			SRV = (ID3D12ShaderResourceView*)resource->additionalSRVs[arrayIndex];
		}

		switch (stage)
		{
		case PRE::VS_STAGE:
			ImmediatedeviceContext->VSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::HS_STAGE:
			ImmediatedeviceContext->HSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::DS_STAGE:
			ImmediatedeviceContext->DSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::GS_STAGE:
			ImmediatedeviceContext->GSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::PS_STAGE:
			ImmediatedeviceContext->PSSetShaderResources(slot, 1, &SRV);
			break;
		case PRE::CS_STAGE:
			ImmediatedeviceContext->CSSetShaderResources(slot, 1, &SRV);
			break;
		default:
			assert(0);
			break;
		}
	}
}

void RenderDevice_DX11::BindResources_Immediate(SHADERSTAGE stage, GPUResource *const* resources, int slot, int count)
{
	assert(count <= 8);
	ID3D12ShaderResourceView* srvs[8];
	for (int i = 0; i < count; ++i)
	{
		srvs[i] = resources[i] != nullptr ? (ID3D12ShaderResourceView*)resources[i]->SRV : nullptr;
	}


	switch (stage)
	{
	case PRE::VS_STAGE:
		ImmediatedeviceContext->VSSetShaderResources(slot, count, srvs);
		break;
	case PRE::HS_STAGE:
		ImmediatedeviceContext->HSSetShaderResources(slot, count, srvs);
		break;
	case PRE::DS_STAGE:
		ImmediatedeviceContext->DSSetShaderResources(slot, count, srvs);
		break;
	case PRE::GS_STAGE:
		ImmediatedeviceContext->GSSetShaderResources(slot, count, srvs);
		break;
	case PRE::PS_STAGE:
		ImmediatedeviceContext->PSSetShaderResources(slot, count, srvs);
		break;
	case PRE::CS_STAGE:
		ImmediatedeviceContext->CSSetShaderResources(slot, count, srvs);
		break;
	default:
		assert(0);
		break;
	}
}

void RenderDevice_DX11::BindUAV_Immediate(SHADERSTAGE stage, GPUResource * resources, int slot, int arrayIndex /*= -1*/)
{
	if (resources != nullptr)
	{
		ID3D12UnorderedAccessView* UAV = (ID3D12UnorderedAccessView*)resources->UAV;

		if (arrayIndex < 0)
		{
			UAV = (ID3D12UnorderedAccessView*)resources->UAV;
		}
		else
		{
			assert(resources->additionalUAVs.size() > static_cast<size_t>(arrayIndex) && "Invalid arrayIndex!");
			UAV = (ID3D12UnorderedAccessView*)resources->additionalUAVs[arrayIndex];
		}

		if (stage == CS_STAGE)
		{
			ImmediatedeviceContext->CSSetUnorderedAccessViews(slot, 1, &UAV, nullptr);
		}
		else
		{
			raster_uavs[ThreadID][slot] = (ID3D12UnorderedAccessView*)resources->UAV;
			raster_uavs_slot[ThreadID] = min(raster_uavs_slot[ThreadID], slot);
			raster_uavs_count[ThreadID] = max(raster_uavs_count[ThreadID], 1);
		}
	}
}

void RenderDevice_DX11::BindUAVS_Immediate(SHADERSTAGE stage, GPUResource* const* resource, int slot, int count)
{
	assert(slot + count <= 8);
	ID3D12UnorderedAccessView* uavs[8];
	for (int i = 0; i < count; ++i)
	{
		uavs[i] = resource[i] != nullptr ? (ID3D12UnorderedAccessView*)resource[i]->UAV : nullptr;

		if (stage != CS_STAGE)
		{
			raster_uavs[ThreadID][slot + i] = uavs[i];
		}
	}

	if (stage == CS_STAGE)
	{
		ImmediatedeviceContext->CSSetUnorderedAccessViews(static_cast<UINT>(slot), static_cast<UINT>(count), uavs, nullptr);
	}
	else
	{
		raster_uavs_slot[ThreadID] = min(raster_uavs_slot[ThreadID], slot);
		raster_uavs_count[ThreadID] = max(raster_uavs_count[ThreadID], count);
	}
}

void RenderDevice_DX11::UnbindResources_Immediate(int slot, int num)
{
	assert(num <= ARRAYSIZE(nullBlob) && "Extend nullBlob to support more resource unbinding");
	ImmediatedeviceContext->PSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	ImmediatedeviceContext->VSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	ImmediatedeviceContext->GSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	ImmediatedeviceContext->HSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	ImmediatedeviceContext->DSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
	ImmediatedeviceContext->CSSetShaderResources(slot, num, (ID3D12ShaderResourceView**)nullBlob);
}

void RenderDevice_DX11::UnbindUAVs_Immediate(int slot, int num)
{
	assert(num <= ARRAYSIZE(nullBlob) && "Extend nullBlob to support more resource unbinding");
	ImmediatedeviceContext->CSSetUnorderedAccessViews(slot, num, (ID3D12UnorderedAccessView**)nullBlob, 0);

	raster_uavs_count[ThreadID] = 0;
	raster_uavs_slot[ThreadID] = 8;
}

void RenderDevice_DX11::BindSampler_Immediate(SHADERSTAGE stage, Sampler* sampler, int slot, int count)
{

	ID3D12SamplerState* SAMPLER = (ID3D12SamplerState*)sampler->resource;
	switch (stage)
	{
	case PRE::VS_STAGE:
		ImmediatedeviceContext->VSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::HS_STAGE:
		ImmediatedeviceContext->HSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::DS_STAGE:
		ImmediatedeviceContext->DSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::GS_STAGE:
		ImmediatedeviceContext->GSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::PS_STAGE:
		ImmediatedeviceContext->PSSetSamplers(slot, count, &SAMPLER);
		break;
	case PRE::CS_STAGE:
		ImmediatedeviceContext->CSSetSamplers(slot, count, &SAMPLER);
		break;
	default:
		assert(0);
		break;
	}
}

void RenderDevice_DX11::BindConstantBuffer_Immediate(SHADERSTAGE stage, GPUBuffer* buffer, int slot, const UINT *pFirstConstant, const UINT *pNumberConstant)
{
	ID3D12Buffer* res = buffer ? (ID3D12Buffer*)buffer->resource : nullptr;
	switch (stage)
	{
	case PRE::VS_STAGE:
		ImmediatedeviceContext->VSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::HS_STAGE:
		ImmediatedeviceContext->HSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::DS_STAGE:
		ImmediatedeviceContext->DSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::GS_STAGE:
		ImmediatedeviceContext->GSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::PS_STAGE:
		ImmediatedeviceContext->PSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	case PRE::CS_STAGE:
		ImmediatedeviceContext->CSSetConstantBuffers1(slot, 1, &res, pFirstConstant, pNumberConstant);
		break;
	default:
		assert(0);
		break;
	}

}

void RenderDevice_DX11::BindVertexBuffers_Immediate(GPUBuffer* const *vertexBuffers, int slot, int count, const UINT* strides, const UINT*offsets)
{
	assert(count < 8);
	ID3D12Buffer* res[8] = { 0 };
	for (int i = 0; i < count; ++i)
	{
		res[i] = vertexBuffers[i] != nullptr ? (ID3D12Buffer*)vertexBuffers[i]->resource : nullptr;
	}
	ImmediatedeviceContext->IASetVertexBuffers(static_cast<UINT>(slot), static_cast<UINT>(count), res, strides, (offsets != nullptr ? offsets : reinterpret_cast<const UINT*>(nullBlob)));
}

void RenderDevice_DX11::BindIndexBuffer_Immediate(GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, UINT offset)
{
	ID3D12Buffer* res = indexBuffer != nullptr ? (ID3D12Buffer*)indexBuffer->resource : nullptr;
	ImmediatedeviceContext->IASetIndexBuffer(res, (format == INDEXBUFFER_FORMAT::INDEXBUFFER_16BIT ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT), offset);
}

void RenderDevice_DX11::BindStencilRef_Immediate(UINT value)
{
	stencilRef[ThreadID] = value;
}

void RenderDevice_DX11::BindBlendFactor_Immediate(XMFLOAT4 value)
{
	blendFactor[ThreadID] = value;
}

void RenderDevice_DX11::BindGraphicsPSO_Immediate(GraphicPSO* pso)
{
	const GraphicsShaderDesc& desc = pso != nullptr ? pso->GetDesc() : GraphicsShaderDesc();

	ID3D12VertexShader* vs = desc.vs == nullptr ? nullptr : (ID3D12VertexShader*)desc.vs->resource;
	if (vs != nullptr)
	{
		ImmediatedeviceContext->VSSetShader(vs, nullptr, 0);
	}
	ID3D12PixelShader* ps = desc.ps == nullptr ? nullptr : (ID3D12PixelShader*)desc.ps->resource;
	if (ps != nullptr)
	{
		ImmediatedeviceContext->PSSetShader(ps, nullptr, 0);
	}
	ID3D12HullShader* hs = desc.hs == nullptr ? nullptr : (ID3D12HullShader*)desc.hs->resource;
	if (hs != nullptr)
	{
		ImmediatedeviceContext->HSSetShader(hs, nullptr, 0);
	}
	ID3D12DomainShader* ds = desc.ds == nullptr ? nullptr : (ID3D12DomainShader*)desc.ds->resource;
	if (ds != nullptr)
	{
		ImmediatedeviceContext->DSSetShader(ds, nullptr, 0);
	}
	ID3D12GeometryShader* gs = desc.gs == nullptr ? nullptr : (ID3D12GeometryShader*)desc.gs->resource;
	if (gs != nullptr)
	{
		ImmediatedeviceContext->GSSetShader(gs, nullptr, 0);
	}

	ID3D12BlendState* bs = desc.bs == nullptr ? nullptr : (ID3D12BlendState*)desc.bs->resource;
	if (bs != nullptr)
	{
		const float fact[4] = { blendFactor[0].x, blendFactor[0].y, blendFactor[0].z, blendFactor[0].w };
		ImmediatedeviceContext->OMSetBlendState(bs, fact, desc.sampleMask);
	}

	ID3D12RasterizerState* rs = desc.rs == nullptr ? nullptr : (ID3D12RasterizerState*)desc.rs->resource;
	if (rs != nullptr)
	{
		ImmediatedeviceContext->RSSetState(rs);
	}

	ID3D12DepthStencilState* dss = desc.dss == nullptr ? nullptr : (ID3D12DepthStencilState*)desc.dss->resource;
	if (dss != nullptr)
	{
		ImmediatedeviceContext->OMSetDepthStencilState(dss, stencilRef[0]);
	}

	ID3D12InputLayout* il = desc.VL == nullptr ? nullptr : (ID3D12InputLayout*)desc.VL->resource;
	if (il != nullptr)
	{
		ImmediatedeviceContext->IASetInputLayout(il);
	}

	{
		D3D12_PRIMITIVE_TOPOLOGY d3dType = D3D12_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		switch (desc.PT)
		{
		case TRIANGELIST:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		case TRIANGESTRIP:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;
		case POINTLIST:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		case LINELIST:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case PATCHLIST:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
			break;
		default:
			d3dType = D3D12_PRIMITIVE_TOPOLOGY_UNDEFINED;
			break;
		};
		ImmediatedeviceContext->IASetPrimitiveTopology(d3dType);
	}
}

void RenderDevice_DX11::BindRasterizerState_Immediate(RasterizerState RSstate)
{
	ImmediatedeviceContext->RSSetState((ID3D12RasterizerState*)RSstate.resource);
}

void RenderDevice_DX11::BindComputerPSO_Immediate(ComputerPSO* pso)
{
	const ComputerPSODesc& desc = pso != nullptr ? pso->GetDesc() : ComputerPSODesc();

	static ID3D12ComputeShader*prev_cs[GRAPHICSTHREAD_COUNT] = {};
	ID3D12ComputeShader* cs = desc.cs == NUll_Handle ? NUll_Handle : (ID3D12ComputeShader*)desc.cs->resource;
	if (cs != prev_cs[ThreadID])
	{
		ImmediatedeviceContext->CSSetShader(cs, nullptr, 0);
		prev_cs[ThreadID] = cs;
	}
}

void RenderDevice_DX11::Draw_Immediate(int vertexCount, UINT startVertexLociotn)
{
	validate_raster_uavs(ThreadID);

	ImmediatedeviceContext->Draw(vertexCount, startVertexLociotn);
}

void RenderDevice_DX11::DrawIndexed_Immediate(UINT Indexcount, UINT startVertexLocation, UINT baseVertexLocation)
{
	validate_raster_uavs(0);

	ImmediatedeviceContext->DrawIndexed(Indexcount, startVertexLocation, baseVertexLocation);
}

void RenderDevice_DX11::DrawInstanced_Immediate(int vertexCount, int instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	validate_raster_uavs(ThreadID);

	ImmediatedeviceContext->DrawInstanced(vertexCount, instanceCount, startVertexLocation, startInstanceLocation);
}

void RenderDevice_DX11::DrawIndexedInstanced_Immediate(int indexCount, int instanceCount, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanced)
{
	validate_raster_uavs(ThreadID);

	ImmediatedeviceContext->DrawIndexedInstanced(indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanced);
}

void RenderDevice_DX11::DrawInstancedIndirect_Immediate(GPUBuffer* args, UINT args_offset)
{
	validate_raster_uavs(ThreadID);

	ImmediatedeviceContext->DrawInstancedIndirect((ID3D12Buffer*)args->resource, args_offset);
}

void RenderDevice_DX11::Dispatch_Immediate(UINT threadGruopCountX, UINT threadGruopCountY, UINT threadGruopCountZ)
{
	ImmediatedeviceContext->Dispatch(threadGruopCountX, threadGruopCountY, threadGruopCountZ);
}

void RenderDevice_DX11::DispatchIndirect_Immediate(GPUBuffer* args, UINT args_offset)
{
	ImmediatedeviceContext->DispatchIndirect((ID3D12Buffer*)args, args_offset);
}

void RenderDevice_DX11::CopyTexture2D_Immediate(Texture2D* pDest, Texture2D* pSrc)
{
	ImmediatedeviceContext->CopyResource((ID3D12Resource*)pDest->resource, (ID3D12Resource*)pSrc->resource);
}

void RenderDevice_DX11::CopyTexture2D_Region_Immediate(Texture2D* pDest, UINT dstMip, UINT dstX, UINT dstY, Texture2D* pSrc, UINT srcMip, UINT ArraySize)
{
	const UINT subresourceIndex = D3D12CalcSubresource(0,ArraySize,PRE::numMipmapLevels(pDest->GetDesc().Width, pDest->GetDesc().Height));
	ImmediatedeviceContext->CopySubresourceRegion((ID3D12Resource*)pDest->resource, subresourceIndex, dstX, dstY, 0,
		(ID3D12Resource*)pSrc->resource, subresourceIndex, nullptr);
}

void RenderDevice_DX11::MSAAResolve_Immediate(Texture2D* pDst, Texture2D* pSrc)
{
	assert(pDst != nullptr && pSrc != nullptr);
	ImmediatedeviceContext->ResolveSubresource((ID3D12Resource*)pDst->resource, 0, (ID3D12Resource*)pSrc->resource, 0, ConvertFormat(pDst->desc.Format));
}


void RenderDevice_DX11::UpdateBuffer_Immediate(GPUBuffer* buffer, const void* data, int datasize)
{
	assert(buffer->desc.Usage != USAGE_IMMUTABLE && "Can not update IMMUTABLE GPUBuffer");
	assert((int)buffer->desc.ByteWidth >= datasize || datasize < 0 && "Data size is too big");

	if (datasize == 0)
	{
		return;
	}
	datasize = min((int)buffer->desc.ByteWidth, datasize);

	if (buffer->desc.Usage == USAGE_DYNAMIC)
	{
		D3D12_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = ImmediatedeviceContext->Map((ID3D12Resource*)buffer->resource, 0, D3D12_MAP_WRITE_DISCARD, 0, &mappedResource);
		assert(SUCCEEDED(hr) && "GPUBuffer mapping failed!");
		memcpy(mappedResource.pData, data, (datasize >= 0 ? datasize : buffer->desc.ByteWidth));
		ImmediatedeviceContext->Unmap((ID3D12Resource*)buffer->resource, 0);
	}
	else if (buffer->desc.BindFlags & BIND_CONSTANT_BUFFER || datasize < 0)
	{
		ImmediatedeviceContext->UpdateSubresource((ID3D12Resource*)buffer->resource, 0, nullptr, data, 0, 0);
	}
	else
	{
		D3D12_BOX box = {};
		box.left = 0;
		box.back = 1;
		box.bottom = 1;
		box.front = 1;
		box.right = static_cast<UINT>(datasize);
		box.top = 0;
		ImmediatedeviceContext->UpdateSubresource((ID3D12Resource*)buffer->resource, 0, &box,data, 0, 0);
	}
}

void * RenderDevice_DX11::AllocateFromRingBuffer_Immediate(GPURingBuffer* buffer, size_t dataSize, UINT& offsetIntoBuffer)
{
	assert(buffer->desc.Usage == USAGE_DYNAMIC && (buffer->desc.CPUAccessFlags &CPU_ACCESS_WRITE) && "Ringbuffer must be write");
	assert(buffer->desc.ByteWidth > dataSize && "Data of the required cannot fit");

	if (dataSize == 0)
	{
		return nullptr;
	}

	dataSize = min(buffer->desc.ByteWidth, dataSize);

	size_t position = buffer->byteOffset;
	bool wrap = position + dataSize > buffer->desc.ByteWidth || buffer->residentFrame != FRAMECOUNT;
	position = wrap ? 0 : position;

	D3D12_MAP maping = wrap ? D3D12_MAP_WRITE_DISCARD : D3D12_MAP_WRITE_NO_OVERWRITE;
	D3D12_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = ImmediatedeviceContext->Map((ID3D12Resource*)buffer->resource, 0, maping, 0, &mappedResource);
	assert(SUCCEEDED(hr) && "GPUBUFFER mapping failed!");

	buffer->byteOffset = position + dataSize;
	buffer->residentFrame = FRAMECOUNT;

	offsetIntoBuffer = (UINT)position;
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(mappedResource.pData) + position);

}

void RenderDevice_DX11::InvalidateBufferAccess_Immediate(GPUBuffer* buffer)
{
	ImmediatedeviceContext->Unmap((ID3D12Resource*)buffer->resource, 0);
}

bool RenderDevice_DX11::DownloadResource_Immediate(GPUBuffer* resurceToDownload, GPUResource* resourceDest, void* dataDest)
{
	{
		GPUBuffer* bufferToDownload = dynamic_cast<GPUBuffer*>(resurceToDownload);
		GPUBuffer* bufferDest = dynamic_cast<GPUBuffer*>(resourceDest);

		if (bufferToDownload != nullptr&& bufferDest != nullptr)
		{
			assert(bufferToDownload->desc.ByteWidth <= bufferDest->desc.ByteWidth);
			assert(bufferDest->desc.Usage& USAGE_STAGING);
			assert(dataDest != nullptr);

			ImmediatedeviceContext->CopyResource((ID3D12Resource*)bufferDest->resource, (ID3D12Resource*)bufferToDownload->resource);

			D3D12_MAPPED_SUBRESOURCE MappedSubresource = {};
			HRESULT hr = ImmediatedeviceContext->Map((ID3D12Resource*)bufferDest->resource, 0, D3D12_MAP_READ,/*asnyc* D3D12_MAP_FLAG_DO_NOT_WAIT*/ 0, &MappedSubresource);
			bool result = SUCCEEDED(hr);
			if (result)
			{
				memcpy(dataDest, MappedSubresource.pData, bufferToDownload->desc.ByteWidth);
				ImmediatedeviceContext->Unmap((ID3D12Resource*)bufferDest->resource, 0);
			}
			return result;
		}
	}

	{
		Texture* textureToDownload = dynamic_cast<Texture*>(resurceToDownload);
		Texture* TextureDest = dynamic_cast<Texture*>(resourceDest);

		if (textureToDownload != nullptr&&TextureDest != nullptr)
		{
			assert(textureToDownload->desc.Width <= TextureDest->desc.Width);
			assert(textureToDownload->desc.Height <= TextureDest->desc.Height);
			assert(textureToDownload->desc.Depth <= TextureDest->desc.Depth);
			assert(TextureDest->desc.Usage& USAGE_STAGING);
			assert(dataDest != nullptr);

			ImmediatedeviceContext->CopyResource((ID3D12Resource*)TextureDest->resource, (ID3D12Resource*)textureToDownload->resource);

			D3D12_MAPPED_SUBRESOURCE mappedResource = {};
			HRESULT hr = ImmediatedeviceContext->Map((ID3D12Resource*)TextureDest->resource, 0, D3D12_MAP_READ, 0, &mappedResource);

			bool result = SUCCEEDED(hr);
			if (result)
			{
				UINT cpycount = max(1, textureToDownload->desc.Width)*max(1, textureToDownload->desc.Height)*max(1, textureToDownload->desc.Depth);
				UINT cpystride = GetFormatStride(textureToDownload->desc.Format);
				UINT cpysize = cpycount * cpystride;
				memcpy(dataDest, mappedResource.pData, cpysize);
				ImmediatedeviceContext->Unmap((ID3D12Resource*)TextureDest->resource, 0);
			}
			return result;
		}
	}
	return false;
}
void RenderDevice_DX11::WaitForGPU_Immediate()
{
	ImmediatedeviceContext->Flush();
}

void RenderDevice_DX11::EventBegin_Immediate(const std::string & name)
{
}

void RenderDevice_DX11::EventEnd_Immediate()
{
}

void RenderDevice_DX11::SetMarker_Immediate(const std::string & name)
{
}

void RenderDevice_DX11::QueryBegin_Immediate(GPUQuery *query)
{
	ImmediatedeviceContext->Begin((ID3D12Query*)query->resources[query->async_frameshift]);
	query->active[query->async_frameshift] = true;
}

void RenderDevice_DX11::QueryEnd_Immediate(GPUQuery* query)
{
	ImmediatedeviceContext->End((ID3D12Query*)query->resources[query->async_frameshift]);
	query->active[query->async_frameshift] = true;
}

bool RenderDevice_DX11::QueryRead_Immediate(GPUQuery* query)
{
	query->async_frameshift = (query->async_frameshift + 1) % (query->desc.async_Latnce + 1);
	const int readQueryID = query->async_frameshift;
	const UINT flag = (query->desc.async_Latnce > 0 ? D3D12_ASYNC_GETDATA_DONOTFLUSH : 0);

	if (!query->active[readQueryID])
	{
		return true;
	}

	assert(ThreadID == GRAPHICTHREAD_IMMERIATE && "A query can only be read on the immediate graphic thread!");

	HRESULT hr = S_OK;
	switch (query->desc.Type)
	{
	case PRE::GPU_QUERY_TYPE_TIMESTAMP:
		hr = ImmediatedeviceContext->GetData((ID3D12Query*)query->resources[readQueryID], &query->result_timestamp, sizeof(query->result_timestamp), flag);
		break;
	case  GPU_QUERY_TYPE_TIMESTAMP_DISJOINT:
	{
		D3D12_QUERY_DATA_TIMESTAMP_DISJOINT temp;
		hr = ImmediatedeviceContext->GetData((ID3D12Query*)query->resources[readQueryID], &temp, sizeof(temp), flag);
		query->result_disjoint = temp.Disjoint;
		query->result_timestamp = temp.Frequency;
	}
	break;
	case GPU_QUERY_TYPE_OCCLUSION:
		hr = ImmediatedeviceContext->GetData((ID3D12Query*)query->resources[readQueryID], &query->result_passed_sample_count, sizeof(query->result_passed_sample_count), flag);
		break;
	case GPU_QUERY_TYPE_OCCLUSION_PREDICATE:
	default:
		hr = ImmediatedeviceContext->GetData((ID3D12Query*)query->resources[readQueryID], &query->result_passed, sizeof(query->result_passed), flag);
		break;
	}

	query->active[readQueryID] = false;
	return hr != S_FALSE;
}

void RenderDevice_DX11::UAVBarrier_Immediate(GPUResource* const* uavs, UINT NumBarries)
{

}

void RenderDevice_DX11::TransitionBarrier_Immediate(GPUResource* const* resource, UINT NumBarrires, RESOUCRCE_STATES stateBefore, RESOUCRCE_STATES stateAFTER)
{

}

void RenderDevice_DX11::GenerateMips_Immediate(GPUResource* ShaderResourceView)
{
	ImmediatedeviceContext->GenerateMips((ID3D12ShaderResourceView*)ShaderResourceView->SRV);
}



      
}

