
#pragma once
#include "CommonInclude.h"
#include "GraphicsDescriptors.h"
#include <memory>
#include <d3d11.h>
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3d11shader.h>
#include <vector>
#include <locale>
#include <codecvt>




typedef uint64_t CPUHandle;
#define NUll_Handle (0)


namespace PRE
{
	struct ShaderInputBindDescDX
	{

		ShaderInputBindDescDX(D3D11_SHADER_INPUT_BIND_DESC desc)
		{
			Name = std::wstring(std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(desc.Name));
			Type = desc.Type;
			BindPoint = desc.BindPoint;
			BindCount = desc.BindCount;
			uFlags = desc.uFlags;
			ReturnType = desc.ReturnType;
			Dimension = desc.Dimension;
			NumSamples = desc.NumSamples;
			//pParamRef = 0;
		};

		ShaderInputBindDescDX()
		{
			Name = std::wstring(L"");
			Type = D3D_SIT_CBUFFER;
			BindPoint = 0;
			BindCount = 0;
			uFlags = 0;
			ReturnType = D3D_RETURN_TYPE_UNORM;
			Dimension = D3D_SRV_DIMENSION_UNKNOWN;
			NumSamples = 0;
			//pParamRef = 0;
		};

		std::wstring Name;
		D3D_SHADER_INPUT_TYPE Type;
		UINT BindPoint;
		UINT BindCount;
		UINT uFlags;
		D3D11_RESOURCE_RETURN_TYPE ReturnType;
		D3D_SRV_DIMENSION Dimension;
		UINT NumSamples;
		//	RenderParameterDX11* pParamRef;

	};

	struct ConstantBufferLayoutDX
	{
		D3D11_SHADER_BUFFER_DESC				Description;
		//std::vector<D3D11_SHADER_VARIABLE_DESC>	Variables;
		//std::vector<D3D11_SHADER_TYPE_DESC>		Types;
		std::vector<UINT> pFirstContantsArray;
		std::vector<UINT> pContantsNumArray;
		UINT BindPoint;
		UINT BindCount;
		ConstantBufferLayoutDX();
	};

	struct ShaderReflectionDX
	{
		std::wstring Name;
		D3D11_SHADER_DESC								ShaderDescription;
		//std::vector<D3D11_SIGNATURE_PARAMETER_DESC>		InputSignatureParameters;
		//std::vector<D3D11_SIGNATURE_PARAMETER_DESC>		OutputSignatureParameters;
		std::vector<ConstantBufferLayoutDX*>				ConstantBuffers;
		std::vector<ShaderInputBindDescDX*>				ResourceBindings;
		ShaderReflectionDX();
	};


	struct ShaderReflection
	{
		ShaderReflection();
		SHADERSTAGE stage;
		CPUHandle ResourceDX = NUll_Handle;
		~ShaderReflection();

	};
	class RenderDevice;

	struct RenderDeviceChild
	{
		RenderDevice* device = nullptr;
		void Register(RenderDevice* dev)
		{
			device = dev;
		}
	};

	struct ShaderByteCode
	{
		BYTE* data;
		size_t size;
		ShaderByteCode() :data(nullptr), size(0)
		{
		
		}
		~ShaderByteCode()
		{
			SAFE_DELETE_ARRAY(data);
		}
	};

	struct GraphicBlob :public RenderDeviceChild
	{
	
		CPUHandle resourceDX=NUll_Handle;

		GraphicBlob();
		~GraphicBlob();
	};

	struct VertexShader:public RenderDeviceChild
	{
		CPUHandle resource = NUll_Handle;
		CPUHandle sf = NUll_Handle;
		VertexShader();
		~VertexShader();
		
	//	ShaderByteCode code;
	};


	struct PixelShader:public RenderDeviceChild
	{
		
		CPUHandle resource = NUll_Handle;
		CPUHandle sf = NUll_Handle;
		PixelShader();
		~PixelShader();
		
	//	ShaderByteCode code;

	};


	struct GeometryShader : public RenderDeviceChild
	{
	
		CPUHandle resource = NUll_Handle;
		GeometryShader();
		~GeometryShader();
		CPUHandle sf = NUll_Handle;
	//	ShaderByteCode code;
	};


	struct HullShader : public RenderDeviceChild
	{
	
		CPUHandle resource = NUll_Handle;
		HullShader();
		~HullShader();
		CPUHandle sf=NUll_Handle;
	//	ShaderByteCode code;
	};

	struct DomainShader : public RenderDeviceChild
	{
		
		CPUHandle resource = NUll_Handle;

		DomainShader();
		~DomainShader();
		CPUHandle sf = NUll_Handle;
	//	ShaderByteCode code;
	};

	struct ComputerShader : public RenderDeviceChild
	{
		
		CPUHandle resource = NUll_Handle;
		ComputerShader();
		~ComputerShader();
		CPUHandle sf = NUll_Handle;
	//	ShaderByteCode code;
	};

	struct Sampler :public RenderDeviceChild
	{
		CPUHandle resource = NUll_Handle;
		SamplerDesc desc;
	
	
	public:
		Sampler();
		~Sampler();

		bool IsValid()
		{
			return resource != NUll_Handle ;
		}
		SamplerDesc GetDesc()
		{
			return desc;
		}
	};


	struct GPUResource :public RenderDeviceChild
	{
		CPUHandle SRV = NUll_Handle;
		std::vector<CPUHandle> additionalSRVs;
		CPUHandle UAV=NUll_Handle;
		std::vector<CPUHandle> additionalUAVs;
		CPUHandle resource;
		CPUHandle resourceMemory;
		GPUResource();
		virtual ~GPUResource();
	};

	struct GPUBuffer :public GPUResource
	{

		CPUHandle CBV=NUll_Handle;
		GPUBufferDesc desc;


		GPUBuffer();
		virtual ~GPUBuffer();

		bool IsVaild()
		{
			return resource!= NUll_Handle;
		}
		GPUBufferDesc GetDesc()
		{
			return desc;
		}
	};

	struct GPURingBuffer :public GPUBuffer
	{
		size_t byteOffset=0;
		uint64_t residentFrame=0;
		size_t GetByteOffset()
		{
			return byteOffset;
		}
	};

	struct VertexLayout : public RenderDeviceChild
	{
		CPUHandle resouce=NUll_Handle;
		std::vector<CPUHandle> desc;
		VertexLayout();
		~VertexLayout();
	};

	struct BlendState :public RenderDeviceChild
	{
	
		CPUHandle resource = NUll_Handle;
		BlendStateDesc desc;

		BlendState();
		~BlendState();
		BlendStateDesc GetDesc()
		{
			return desc;
		}

	};

	struct DepthStencilState :public RenderDeviceChild
	{
	
		CPUHandle resource=NUll_Handle;
		DepthStencilStateDesc desc;

		DepthStencilState();
		~DepthStencilState();

		DepthStencilStateDesc GetDesc()
		{
			return desc;
		}
	};
	
	struct RasterizerState:public RenderDeviceChild
	{
		
		CPUHandle resource=NUll_Handle;
		RasterizerStateDesc desc;
		RasterizerState();
		~RasterizerState();

		RasterizerStateDesc GetDesc()
		{
			return desc;
		}
	};

	struct Texture :public GPUResource
	{

		TextureDesc desc;
		CPUHandle RTV=NUll_Handle;
		std::vector<CPUHandle> additionalRTVs;
		bool independentRTVArraySlices=false;
		bool independentRTVCubemapFaces = false;
		bool independentSRVArraySlices = false;
		bool indepentdentSRVMIPs = false;
		bool indepentdentUAVMIPs = false;
	public:
		const TextureDesc& GetDesc()
		{
			return desc;
		}

		Texture();
		virtual ~Texture();


		void RequestIndependentRenderTargetArraySlices(bool value);

		void RequestIndepentRenderTargetCubemapFaces(bool value);

		void RequestIndepentShaderResourceArraySlices(bool value);

		void RequestIndepentShaderReourcesForMIPs(bool value);

		void RequesIndenpentUnorderedAccessResoucesForMips(bool value);

	};

	struct  Texture1D:public Texture
	{

		Texture1D();
		virtual ~Texture1D();
	};

	struct Texture2D :public Texture
	{
	
		CPUHandle DSV=NUll_Handle;
		std::vector<CPUHandle> additionalDSVs;
	

	public:
		Texture2D();
		virtual ~Texture2D();
	};

	struct Texture3D :public Texture
	{
	public:
		Texture3D();
		virtual~Texture3D();
	};

	struct GPUQuery :public RenderDeviceChild
	{
	
		std::vector<CPUHandle> resource;
		std::vector<int> active;
		GPUQueryDesc desc;
		int async_frameshift;


		GPUQuery();
		virtual ~GPUQuery();

		bool IsValid()
		{
			return !resource.empty() && resource[0] != NUll_Handle;
		}

		GPUQueryDesc GetDesc() const {
			return desc;
		}

		BOOL result_passed;
		UINT64 result_passed_sample_count;
		UINT64 result_timestamp;
		UINT64 result_timestamp_frequency;
		BOOL result_disjoint;
	};

	struct  GraphicPSO :public RenderDeviceChild
	{
	
		CPUHandle pipeline;
		GraphicsShaderDesc desc;

	
		const GraphicsShaderDesc& GetDesc() const
		{
			return desc;
		}

		GraphicPSO();
		~GraphicPSO();
	};

	struct  ComputerPSO :public RenderDeviceChild
	{
	
		CPUHandle pipeline;
		ComputerPSODesc desc;


		const ComputerPSODesc& GetDesc() const
		{
			return desc;
		}
		ComputerPSO();
		~ComputerPSO();
	};

	
}