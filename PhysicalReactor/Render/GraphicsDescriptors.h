#pragma once
#include "CommonInclude.h"


namespace PRE
{
	struct Shader;
	struct BlendState;
	struct RasterizerState;
	struct DepthStencilState;
	struct VertexLayout;
	struct GPUResource;
	struct GPUBuffer;
	struct Texture;

	enum SHADERSTAGE
	{
		VS_STAGE,
		HS_STAGE,
		DS_STAGE,
		GS_STAGE,
		PS_STAGE,
		CS_STAGE,
		STAGE_COUNT
	};

	enum PRIMITIVEOPOLOGY
	{
		UNDEFINED_TOPOLOGY,
		TRIANGELIST,
		TRIANGESTRIP,
		POINTLIST,
		LINELIST,
		LINESTRIP,
		PATCHLIST,
	};
	
	enum ShaderMask
	{
		VERTEX_SHADER_MSK=0x0001,
		HULL_SHADER_MSK=0x0002,
		DOMAIN_SHADER_MSK=0x0003,
		GEOMETRY_SHADER_MSK=0x0004,
		PIXEL_SHADER_MSK=0x0010,
		COMPUTE_SHADER_MSK=0X0020
	};

	
	enum COMPARSION_FUNC
	{
		COMPARISON_NEVER,
		COMPARISON_LESS,
		COMPARISON_EQUAL,
		COMPARISON_LESS_EQUAL,
		COMPARISON_GREATER,
		COMPARISON_NOT_EQUAL,
		COMPARISON_GREATER_EQUAL,
		COMPARISON_ALWAYS,
	};

	enum DEPTH_WRITE_MASK
	{
		DEPTH_WRITE_MASK_ZERO,
		DEPTH_WRITE_MASK_ALL,
	};

	enum STENCIL_OP
	{
		STENCIL_OP_KEEP,
		STENCIL_OP_ZERO,
		STENCIL_OP_REPLACE,
		STENCIL_OP_INCR_SAT,
		STENCIL_OP_DECR_SAT,
		STENCIL_OP_INVERT,
		STENCIL_OP_INCR,
		STENCIL_OP_DECR,
	};

	enum BLEND
	{
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRC_COLOR,
		BLEND_INV_SRC_COLOR,
		BLEND_SRC_ALPHA,
		BLEND_INV_SRC_ALPHA,
		BLEND_DEST_ALPHA,
		BLEND_INV_DEST_ALPHA,
		BLEND_DEST_COLOR,
		BLEND_INV_DEST_COLOR,
		BLEND_SRC_ALPHA_SAT,
		BLEND_BLEND_FACTOR,
		BLEND_INV_BLEND_COLOR,
		BLEND_SRC1_COLOR,
		BLEND_INV_SRC1_COLOR,
		BLEND_SRC1_ALPHA,
		BLEND_INV_SRC1_ALPHA,
	};
	
	enum COLOR_WRITE_ENABLE
	{
		COLOR_WRITE_DISABLE = 0,
	    COLOR_WRITE_ENABLE_RED=1,
		COLOR_WRITE_ENABLE_GREEN=2,
		COLOR_WRITE_ENABLE_BLUE=4,
		COLOR_WRITE_ENABLE_ALPHA=8,
		COLOR_WRITE_ENABLE_ALL=(((COLOR_WRITE_ENABLE_RED|COLOR_WRITE_ENABLE_GREEN)|COLOR_WRITE_ENABLE_BLUE)|COLOR_WRITE_ENABLE_ALPHA)
	};

	enum BLEND_OP
	{
		BLEND_OP_ADD,
		BLEND_OP_SUBTRACT,
		BLEND_OP_REV_SUBTRACT,
		BLEND_OP_MIN,
		BLEND_OP_MAX,
	};

	enum FILL_MODE
	{
		FILL_WIREFRAME,
		FILL_SOLID,
	};

	enum CULL_MODE
	{
		CULL_NONE,
		CULL_FRONT,
		CULL_BACK,
	};

	enum INPUT_CLASSIFICATION
	{
		INPUT_PER_VERTEX_DATA,
		INPUT_PER_INSTANCE_DATA,
	};

	enum USAGE
	{
		USAGE_DEFAULT,
		USAGE_IMMUTABLE,
		USAGE_DYNAMIC,
		USAGE_STAGING,
	};

	enum TEXTURE_ADDRESS_MODE
	{
		TEXTURE_ADDRESS_WRAP,
		TEXTURE_ADDRESS_MIRROR,
		TEXTURE_ADDRESS_CLAMP,
		TEXTURE_ADDRESS_BORDER,
		TEXTURE_ADDRESS_MIRROR_ONCE,
	};

	enum FILTER
	{
		FILTER_MIN_MAG_MIP_POINT,
		FILTER_MIN_MAG_POINT_MIP_LINEAR,
		FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
		FILTER_MIN_POINT_MAG_MIP_LINEAR,
		FILTER_MIN_LINEAR_MAG_MIP_POINT,
		FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		FILTER_MIN_MAG_LINEAR_MIP_POINT,
		FILTER_MIN_MAG_MIP_LINEAR,
		FILTER_ANISOTROPIC,
		FILTER_COMPARISON_MIN_MAG_MIP_POINT,
		FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
		FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
        FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
		FILTER_COMPARSION_LINEAR_MAG_MIP_POINT,
		FILTER_COMPARSION_LINEAR_MAG_POINT_MIP_LINEAR,
		FILTER_COMPARSION_MIN_MAG_LINEAR_MIP_POINT,
		FILTER_COMPARSION_MIN_MAG_MIP_LINEAR,
		FILTER_COMPARSION_ANISOTROPIC,
		FILTER_MINIMUM_MIN_MAG_MIP_POINT,
		FILTER_MINMUM_MIN_MAG_POINT_MIP_LINEAR,
		FILTER_MINMUM_MIN_POINT_MAG_POINT_LINEAR_MIP_POINT,
		FILTER_MINMUM_MIN_POINT_MAG_MIP_LINEAR,
		FILTER_MINMUM_MIN_LINEAR_MAG_MIP_POINT,
		FILTER_MINMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		FILTER_MINMUM_MIN_MAG_LINEAR_MIP_POINT,
		FILTER_MINMUM_MIN_MAG_MIN_LINEAR,
		FILTER_MINMUM_ANSOTROPIC,
		FILTER_MAXIMUM_MIN_MAG_MIP_POINT,
		FILTER_MAXIMUM_MIN_MAG_POINT_MIP_POINT,
		FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
		FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,
		FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,
		FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,
		FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR,
		FILTER_MAXIMUM_ANISOTROPIC,
	};

	enum FORMAT
	{
		FORMAT_UNKNOWN,
		FORMAT_R32G32B32A32_TYPELESS,
		FORMAT_R32G32B32A32_FLOAT,
		FORMAT_R32G32B32A32_UINT,
		FORMAT_R32G32B32A32_SINT,
		FORMAT_R32G32B32_TYPELESS,
		FORMAT_R32G32B32_FLOAT,
		FORMAT_R32G32B32_UINT,
		FORMAT_R32G32B32_SINT,
		FORMAT_R16G16B16A16_TYPELESS,
		FORMAT_R16G16B16A16_FLOAT,
		FORMAT_R16G16B16A16_UNORM,
		FORMAT_R16G16B16A16_UINT,
		FORMAT_R16G16B16A16_SNORM,
		FORMAT_R16G16B16A16_SINT,
		FORMAT_R32G32_TYPELESS,
		FORMAT_R32G32_FLOAT,
		FORMAT_R32G32_UINT,
		FORMAT_R32G32_SINT,
		FORMAT_R32G8X24_TYPELESS,
		FORMAT_D32_FLOAT_S8X24_UINT,
		FORMAT_R32_FLOAT_X8X24_TYPELESS,
		FORMAT_X32_TYPELESS_G8X24_UINT,
		FORMAT_R10G10B10A2_TYPELESS,
		FORMAT_R10G10B10A2_UNORM,
		FORMAT_R10G10B10A2_UINT,
		FORMAT_R11G11B10_FLOAT,
		FORMAT_R8G8B8A8_TYPELESS,
		FORMAT_R8G8B8A8_UNORM,
		FORMAT_R8G8B8A8_UNORM_SRGB,
		FORMAT_R8G8B8A8_UINT,
		FORMAT_R8G8B8A8_SNORM,
		FORMAT_R8G8B8A8_SINT,
		FORMAT_R16G16_TYPELESS,
		FORMAT_R16G16_FLOAT,
		FORMAT_R16G16_UNORM,
		FORMAT_R16G16_UINT,
		FORMAT_R16G16_SNORM,
		FORMAT_R16G16_SINT,
		FORMAT_R32_TYPELESS,
		FORMAT_D32_FLOAT,
		FORMAT_R32_FLOAT,
		FORMAT_R32_UINT,
		FORMAT_R32_SINT,
		FORMAT_R24G8_TYPELESS,
		FORMAT_D24_UNORM_S8_UINT,
		FORMAT_R24_UNORM_X8_TYPELESS,
		FORMAT_X24_TYPELESS_G8_UINT,
		FORMAT_R8G8_TYPELESS,
		FORMAT_R8G8_UNORM,
		FORMAT_R8G8_UINT,
		FORMAT_R8G8_SNORM,
		FORMAT_R8G8_SINT,
		FORMAT_R16_TYPELESS,
		FORMAT_R16_FLOAT,
		FORMAT_D16_UNORM,
		FORMAT_R16_UNORM,
		FORMAT_R16_UINT,
		FORMAT_R16_SNORM,
		FORMAT_R16_SINT,
		FORMAT_R8_TYPELESS,
		FORMAT_R8_UNORM,
		FORMAT_R8_UINT,
		FORMAT_R8_SNORM,
		FORMAT_R8_SINT,
		FORMAT_A8_UNORM,
		FORMAT_R1_UNORM,
		FORMAT_R9G9B9E5_SHAREDEXP,
		FORMAT_R8G8_B8G8_UNORM,
		FORMAT_G8R8_G8B8_UNORM,
		FORMAT_BC1_TYPELESS,
		FORMAT_BC1_UNORM,
		FORMAT_BC1_UNORM_SRGB,
		FORMAT_BC2_TYPELESS,
		FORMAT_BC2_UNORM,
		FORMAT_BC2_UNORM_SRGB,
		FORMAT_BC3_TYPELESS,
		FORMAT_BC3_UNORM,
		FORMAT_BC3_UNORM_SRGB,
		FORMAT_BC4_TYPELESS,
		FORMAT_BC4_UNORM,
		FORMAT_BC4_SNORM,
		FORMAT_BC5_TYPELESS,
		FORMAT_BC5_UNORM,
		FORMAT_BC5_SNORM,
		FORMAT_B5G6R5_UNORM,
		FORMAT_B5G5R5A1_UNORM,
		FORMAT_B8G8R8A8_UNORM,
		FORMAT_B8G8R8X8_UNORM,
		FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
		FORMAT_B8G8R8A8_TYPELESS,
		FORMAT_B8G8R8A8_UNORM_SRGB,
		FORMAT_B8G8R8X8_TYPELESS,
		FORMAT_B8G8R8X8_UNORM_SRGB,
		FORMAT_BC6H_TYPELESS,
		FORMAT_BC6H_UF16,
		FORMAT_BC6H_SF16,
		FORMAT_BC7_TYPELESS,
		FORMAT_BC7_UNORM,
		FORMAT_BC7_UNORM_SRGB,
		FORMAT_AYUV,
		FORMAT_Y410,
		FORMAT_Y416,
		FORMAT_NV12,
		FORMAT_P010,
		FORMAT_P016,
		FORMAT_420_OPAQUE,
		FORMAT_YUY2,
		FORMAT_Y210,
		FORMAT_Y216,
		FORMAT_NV11,
		FORMAT_AI44,
		FORMAT_IA44,
		FORMAT_P8,
		FORMAT_A8P8,
		FORMAT_B4G4R4A4_UNORM,
		FORMAT_FORCE_UINT = 0xffffffff,
	};

	enum GPU_QUERY_TYPE
	{
		GPU_QUERY_TYPE_INVALID,
		GPU_QUERY_TYPE_EVENT,
		GPU_QUERY_TYPE_OCCLUSION,
		GPU_QUERY_TYPE_OCCLUSION_PREDICATE,
		GPU_QUERY_TYPE_TIMESTAMP,
		GPU_QUERY_TYPE_TIMESTAMP_DISJOINT,
	};

	enum INDEXBUFFER_FORMAT
	{
		INDEXBUFFER_16BIT,
		INDEXBUFFER_32BIT,
	};


	enum SUBRESOURCE_TYPE
	{
		SRV,
		UAV,
		RTV,
		DSV,
	};

	enum IMAGE_LAYOUT
	{
	   IMAGE_LAYOUT_UNDEFINED,
	   IMAGE_LAYOUT_GENERAL,
	   IMAGE_LAYOUT_RENDERTRGET,
	   IMAGE_LAYOUT_DEPTHSTENCIL,
	   IMAGE_LAYOUT_DEPTHSTENCIL_READONLY,
	   IMAGE_LAYOUT_SHADER_RESOURCE,
	   IMAGE_LAYOUT_UNORDERED_ACCESS,
	   IMAGE_LAYOUT_COPY_SRC,
	   IMAGE_LAYOUT_COPY_DEST,
	
	};

	enum BUFFER_STATE
	{
		BUFFER_STATE_GENERAL,
		BUFFER_STATE_VERTEX_BUFFER,
		BUFFER_STATE_INDEX_BUFFER,
		BUFFER_STATE_CONSTANT_BUFFER,
		BUFFER_STATE_INDIRECT_ARGUMENT,
		BUFFER_STATE_SHADER_RESOURCE,
		BUFFER_STATE_UNORDERED_ACCESS,
		BUFFER_STATE_COPY_SRC,
		BUFFER_STATE_COPY_DST,

	};

	enum CLEAR_FLAG
	{
		CLEAR_DEPTH=0x1L,
		CLEAR_STENCIL=0x2L,
	};

	enum BIND_FLAG
	{
		BIND_VERTEX_BUFFER = 0x1L,
		BIND_INDEX_BUFFER = 0X2L,
		BIND_CONSTANT_BUFFER = 0x4L,
		BIND_SHADER_RESOURCE = 0x8L,
		BIND_STREAM_OUTPUT = 0x10L,
		BIND_RENDER_TARGET = 0x20L,
		BIND_DEPTH_STENCIL = 0x40L,
		BIND_UNORDERED_ACCESS = 0x80L,
	};

	enum CPU_ACCESS
	{
		CPU_ACCESS_WRITE=0x10000L,
	    CPU_ACCESS_READ=0x20000L,
	};

	enum RESOURCE_MISC_FLAG
	{
		RESOURCE_MISC_GENERATE_MIPS = 0x1L,
		RESOURCE_MISC_SHARED = 0x2L,
		RESOURCE_MISC_TEXTURECUBE = 0x4L,
		RESOURCE_MISC_DRAWINDIRECT_ARGS = 0x10L,
		RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS = 0x20L,
		RESOURCE_MISC_BUFFER_STRUCTURED = 0x40L,
		RESOURCE_MISC_RESOURCE_CLAMP = 0x80L,
		RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x100L,
		RESOURCE_MISC_GDI_COMPATIBLE = 0x200L,
		RESOURCE_MISC_SHARED_NTHANDLE = 0x800L,
		RESOURCE_MISC_RESTRICTED_CONTENT = 0x1000L,
		RESOURCE_MISC_RESTRICT_SHARED_RESOURCE = 0x2000L,
		RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER = 0x4000L,
		RESOURCE_MISC_GUARDED = 0x8000L,
		RESOURCE_MISC_TILE_POOL = 0x20000L,
		RESOURCE_MISC_TILED = 0x40000L,
		RESOURCE_MISC_HW_PROTECTED = 0x80000L
	};

	enum RESOUCRCE_STATES
	{
		RESOUCRCE_STATE_COMMON = 0,
		RESOUCRCE_STATES_VERTEX_AND_CONSTANT_BUFFER = 0x1,
		RESOUCRCE_STATES_INDEX_BUFFER = 0x2,
		RESOUCRCE_STATES_RENDER_TARGET = 0x4,
		RESOUCRCE_STATES_UNORODERED_ACCESS = 0x8,
		RESOUCRCE_STATES_DEPTH_WRITE = 0x10,
		RESOUCRCE_STATES_DEPTH_READ = 0x20,
		RESOUCRCE_STATES_NON_PIXEL_SHADER_RESOURCE = 0x40,
		RESOURCE_STATE_PIXEL_SHADER_RESOURCE = 0x80,
		RESOURCE_STATE_STREAM_OUT = 0x100,
		RESOURCE_STATE_INDIRECT_ARGUMENT = 0x200,
		RESOURCE_STATE_COPY_DEST = 0x400,
		RESOURCE_STATE_COPY_SOURCE = 0x800,
		RESOURCE_STATE_RESOLVE_DEST = 0x1000,
		RESOURCE_STATE_RESOLVE_SOURCE = 0x2000,
		RESOURCE_STATE_GENERIC_READ = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
		RESOURCE_STATE_PRESENT = 0,
		RESOURCE_STATE_PREDICATION = 0x200,
		RESOURCE_STATE_VIDEO_DECODE_READ = 0x10000,
		RESOURCE_STATE_VIDEO_DECODE_WRITE = 0x20000,
		RESOURCE_STATE_VIDEO_PROCESS_READ = 0x40000,
		RESOURCE_STATE_VIDEO_PROCESS_WRITE = 0x80000
	};






#define APPEND_ALIGNED_ELEMENT (0xffffffff)
#define  FLOAT32_MAX ( 3.402823466e+38f )
#define  DEFAULT_STENCIL_READ_MASK (Oxff)
#define  SO_NO_RASTERIZED_STREAM (0xffffffff)
	  
	struct ViewPort
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;

		ViewPort() :TopLeftX(0.0f), TopLeftY(0.0f), Width(0.0f), Height(0.0f), MinDepth(0.0f), MaxDepth(1.0f) {};
	};

	struct VertexLayoutDesc
	{
		const char* SemanticName;
		UINT SemanticIndex;
		FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
	};

	struct SampleDesc
	{
		UINT Count;
	    UINT Quality;

		SampleDesc() :Count(1), Quality(0) {}
	};

	struct TextureDesc
	{
		enum TEXTURE_TYPE
		{
			TEXTURE_1D,
			TEXTURE_2D,
			TEXTURE_3D,
		} type = TEXTURE_2D;
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Depth = 0;
		uint32_t ArraySize = 1;
		uint32_t MipLevels = 1;
		FORMAT Format = FORMAT_UNKNOWN;
		uint32_t SampleCount = 1;
		USAGE Usage = USAGE_DEFAULT;
		uint32_t BindFlags = 0;
		uint32_t CPUAccessFlags = 0;
		uint32_t MiscFlags = 0;
		ClearValue clear = {};
		IMAGE_LAYOUT layout = IMAGE_LAYOUT_GENERAL;
	};

	struct SamplerDesc
	{
		FILTER Filter;
		TEXTURE_ADDRESS_MODE AddressU;
		TEXTURE_ADDRESS_MODE AddressV;
		TEXTURE_ADDRESS_MODE AddressW;
		float MipLODBias;
		UINT MaxAnisotropy;
		COMPARSION_FUNC ComparsionFunc;
		float BorderColor[4];
		float MinLOD;
		float MaxLOD;


		SamplerDesc():
			Filter(FILTER_MIN_MAG_MIP_POINT),
			AddressU(TEXTURE_ADDRESS_CLAMP),
			AddressV(TEXTURE_ADDRESS_CLAMP),
			AddressW(TEXTURE_ADDRESS_WRAP),
			MipLODBias(0.0f),
			MaxAnisotropy(0),
			ComparsionFunc(COMPARISON_NEVER),
			BorderColor{0.0f,0.0f,0.0f,0.0f},
			MinLOD(0.0f),
			MaxLOD(FLT_MAX){}
	};

	struct RasterizerStateDesc
	{
		FILL_MODE FillMode;
		CULL_MODE CullMode;
		bool FrontCounterClockWise;
		INT DepthBias;
		float DepthBiasClamp;
		float SlopeScaledDepthBias;
		bool DepthCilpEnable;
		bool ScissorEnable;
		bool MultisampleEnable;
		bool AntialiasedLineEnable;
		bool ConservativeRasterizationEnable;
		UINT ForceSampleCount;

		RasterizerStateDesc() :FillMode(FILL_SOLID), CullMode(CULL_NONE), FrontCounterClockWise(false), DepthBias(0), DepthBiasClamp(0.0f),
			SlopeScaledDepthBias(0.0f), DepthCilpEnable(false), ScissorEnable(false), MultisampleEnable(false), AntialiasedLineEnable(false), ConservativeRasterizationEnable(false), ForceSampleCount(0)
		{}
	};

	struct DepthStencilOpDesc
	{
		STENCIL_OP StencilFailOp;
		STENCIL_OP StencilDepthFailOp;
		STENCIL_OP StencilPassOp;
		COMPARSION_FUNC StencilFunc;

		DepthStencilOpDesc() :
			StencilFailOp(STENCIL_OP_KEEP),
			StencilDepthFailOp(STENCIL_OP_KEEP),
			StencilPassOp(STENCIL_OP_KEEP),
			StencilFunc(COMPARISON_NEVER)
		{}
	};

	struct DepthStencilStateDesc
	{
		bool DepthEnable;
		DEPTH_WRITE_MASK DepthWriteMask;
		COMPARSION_FUNC DepthFunc;
		bool StencilEnable;
		UINT8 StencilReadMask;
		UINT8 StencilWriteMask;
		DepthStencilOpDesc FrontFace;
		DepthStencilOpDesc BackFace;


		DepthStencilStateDesc() :DepthEnable(false), DepthWriteMask(DEPTH_WRITE_MASK_ZERO), DepthFunc(COMPARISON_NEVER),
			StencilEnable(false), StencilReadMask(0xff),  StencilWriteMask(0xff) {}

	};

	struct RenderTargetBlendSateDesc
	{

		bool BlendEnable;
		BLEND SrcBlend;
		BLEND DestBlend;
		BLEND_OP BlendOp;
		BLEND SrcBlendAlpha;
		BLEND DestBlendAlpha;
		BLEND_OP BlendOpAlpha;
		UINT8 RenderTargetWriteMask;

		RenderTargetBlendSateDesc() :BlendEnable(false), SrcBlend(BLEND_SRC_ALPHA), DestBlend(BLEND_INV_SRC_ALPHA), BlendOp(BLEND_OP_ADD),
			SrcBlendAlpha(BLEND_ONE), DestBlendAlpha(BLEND_ONE), BlendOpAlpha(BLEND_OP_ADD), RenderTargetWriteMask(COLOR_WRITE_ENABLE_ALL)
		{}
	};

	struct BlendStateDesc
	{
		bool AlphaToConvrageEnable;
		bool IndependentBlendEnable;
		RenderTargetBlendSateDesc RenderTarget[8];

		BlendStateDesc():
			AlphaToConvrageEnable(false),
			IndependentBlendEnable(false)
		{}
	};

	struct GPUBufferDesc
	{
		UINT ByteWidth;
		USAGE Usage;
		UINT BindFlags;
		UINT CPUAccessFlags;
		UINT MiscFlags;
		UINT StructureByteStride;
		FORMAT Format;

		GPUBufferDesc() :ByteWidth(0), Usage(USAGE_DEFAULT), BindFlags(0), CPUAccessFlags(0), MiscFlags(0),
			StructureByteStride(0), Format(FORMAT_UNKNOWN) {}	
	};

	struct GPUQueryDesc
	{
		GPU_QUERY_TYPE Type;
		

		GPUQueryDesc() :Type(GPU_QUERY_TYPE_INVALID)

		{}
	};

	struct GPUQueryResult
	{
		uint64_t result_passed_sample_coount;
		uint64_t result_timestamp;
		uint64_t result_timestamp_frequency;

		GPUQueryResult() :result_passed_sample_coount(0), result_timestamp(0), result_timestamp_frequency(0) {}
	};

	struct PipelineStateDesc
	{
		const Shader*				vs = nullptr;
		const Shader*				ps = nullptr;
		const Shader*				hs = nullptr;
		const Shader*				ds = nullptr;
		const Shader*				gs = nullptr;
		const BlendState*			bs = nullptr;
		const RasterizerState*		rs = nullptr;
		const DepthStencilState*	dss = nullptr;
		const VertexLayout*			il = nullptr;
		PRIMITIVEOPOLOGY			pt = TRIANGELIST;
		uint32_t					sampleMask = 0xFFFFFFFF;


	};
	/*struct GraphicsShaderDesc
	{
		VertexShader* vs;
		PixelShader* ps;
		HullShader* hs;
		DomainShader* ds;
		GeometryShader* gs;
		BlendState* bs;
		RasterizerState* rs;
		DepthStencilState* dss;
		VertexLayout* VL;
		PRIMITIVEOPOLOGY PT;
		UINT numRTs;
		FORMAT RTFormats[8];
		FORMAT DSFormat;
		SampleDesc sampleDesc;
		UINT sampleMask;

		GraphicsShaderDesc()
		{
			SAFE_INITIL(vs);
			SAFE_INITIL(ps);
			SAFE_INITIL(hs);
			SAFE_INITIL(ds);
			SAFE_INITIL(gs);
			SAFE_INITIL(bs);
			SAFE_INITIL(rs);
			SAFE_INITIL(dss);
			SAFE_INITIL(VL);

			PT = TRIANGELIST;
			numRTs = 0;
			for (int i=0;i<ARRAYSIZE(RTFormats);++i)
			{
				RTFormats[i] = FORMAT_UNKNOWN;
			}
			DSFormat = FORMAT_UNKNOWN;
			sampleDesc.Count = 1;
			sampleDesc.Quality = 0;
			sampleMask = 0xFFFFFFFF;
		}
	};*/

	/*struct ComputerPSODesc
	{
		ComputerShader *cs;
		ComputerPSODesc()
		{
			SAFE_INITIL(cs);
		}
	};*/

	struct IndirectDrawArgsInstanced
	{
		UINT VertexCountPerInstance;
		UINT InstanceCount;
		UINT StartVertexLoction;
		UINT StartInstanceLoction;

		IndirectDrawArgsInstanced() :
			VertexCountPerInstance(0), InstanceCount(0), StartVertexLoction(0), StartInstanceLoction(0) {}
	};

	struct IndirectDrawArgsIndexedInstanced
	{
		UINT IndexCountPerInstance;
		UINT InstanceCount;
		UINT StartIndexLocation;
		INT BaseVertexLocation;
		UINT StartInstanceLocation;

		IndirectDrawArgsIndexedInstanced() :
			IndexCountPerInstance(0),
			InstanceCount(0),
			StartIndexLocation(0),
			BaseVertexLocation(0),
			StartInstanceLocation(0){}
	};

	struct IndirectDispacthArgs
	{
		UINT ThreadGroupCountX;
		UINT ThreadGroupCountY;
		UINT ThreadGroupCountZ;

		IndirectDispacthArgs() :
			ThreadGroupCountX(0),
			ThreadGroupCountY(0),
			ThreadGroupCountZ(0)
		{}
	};

	struct SubresourceData
	{
		const void *pSysMem;
		UINT SysMemPitch;
		UINT SysMemSlicePitch;

		SubresourceData() :
			pSysMem(nullptr),
			SysMemPitch(0),
			SysMemSlicePitch(0)
		{}
	};

	struct MappedSubresource
	{
		void* pData;
		UINT RowPitch;
		UINT DepthPitch;

		MappedSubresource() :
			pData(nullptr),
			RowPitch(0),
			DepthPitch(0) {}
	};

	struct Rect
	{
		LONG left;
		LONG top;
		LONG right;
		LONG bottom;

		Rect():left(0),top(0),right(0),bottom(0)
		{}
	};

	struct GPUBarrier
	{
		enum TYPE
		{
			MEMORY_BARRIER,
			IMAGE_BARRIER,
			BUFFER_BARRIER,
		}type = MEMORY_BARRIER;
		union 
		{
			struct Memory
			{
				const GPUResource* resource;
			}memory;
			struct Image
			{

				const Texture* texture;
				IMAGE_LAYOUT layout_before;
				IMAGE_LAYOUT layout_after;
			}image;
			struct Buffer
			{
				const GPUBuffer* buffer;
				BUFFER_STATE state_before;
				BUFFER_STATE state_after;

			}buffer;

		};
		static GPUBarrier Memory(const GPUResource* resource = nullptr)
		{
			GPUBarrier barrier;
			barrier.type = MEMORY_BARRIER;
			barrier.memory.resource = resource;
			return barrier;
		}

		static GPUBarrier Image(const Texture* texture, IMAGE_LAYOUT before, IMAGE_LAYOUT after)
		{
			GPUBarrier barrier;
			barrier.type = IMAGE_BARRIER;
			barrier.image.texture = texture;
			barrier.Image.layout_before = before;
			barrier.image.layout_after = after;
			return barrier;
		
		}
		static GPUBarrier Buffer(const GPUBuffer* buffer, BUFFER_STATE before, BUFFER_STATE after)
		{
			GPUBarrier barrier;
			barrier.type = BUFFER_BARRIER;
			barrier.buffer.buffer = buffer;
			barrier.buffer.state_before = before;
			barrier.buffer.state_after = after;
			return barrier;
		
		}
	};


	struct RenderPassAttachment
	{

		enum TYPE
		{
			RENDERTARGET,
			DEPTH_STENCIL,
		}type = RENDERTARGET;
		enum LOAD_OPERATION
		{
			LOADOP_LOAD,
			LOADOP_CLEAR,
			LOADOP_DONTCARE,
		}loadop = LOADOP_LOAD;
		const Texture* texture = nullptr;
		int subresource = -1;
		enum STORE_OPERATION
		{
			STOREOP_STORE,
			STOREOP_DONTCARE,

		}storeop = STOREOP_STORE;
		IMAGE_LAYOUT initial_layout = IMAGE_LAYOUT_GENERAL;
		IMAGE_LAYOUT final_layout = IMAGE_LAYOUT_GENERAL;


	};

	struct RenderPassDesc
	{
		uint32_t numAttachments = 0;
		RenderPassAttachment attachments[9] = {};
	};

	struct IndirectDrawArgsInstanced
	{
		uint32_t VertexCountPerInstance = 0;
		uint32_t InstanceCount = 0;
		uint32_t StartVertexLocation = 0;
		uint32_t StartInstanceLocation = 0;
	};

	struct IndirectDispatchArgs
	{
		uint32_t ThreadGroupCountX = 0;
		uint32_t ThreadGroupCountY = 0;
		uint32_t ThreadGroupCountZ = 0;
	};



	
}