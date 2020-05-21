
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

	class RenderDevice;

	struct RenderDeviceChild
	{
		std::shared_ptr<void> internal_state;
		inline bool IsVaild() const
		{
			return internal_state.get() != nullptr;
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
	
	

		GraphicBlob();
		~GraphicBlob();
	};

	struct Shader:public RenderDeviceChild
	{
		
		SHADERSTAGE stage = STAGE_COUNT;
		std::vector<uint8_t> code;
		
	
	};



	struct Sampler :public RenderDeviceChild
	{
		SamplerDesc desc;

	    const SamplerDesc& GetDesc() const
		{
			return desc;
		}
	};


	struct GPUResource :public RenderDeviceChild
	{
		enum class GPU_RESOURCE_TYPE
		{
			BUFFER,
			TEXTURE,
			UNKONW_TYPE,
		}type = GPU_RESOURCE_TYPE::UNKONW_TYPE;

		inline bool IsTexture() const { return type == GPU_RESOURCE_TYPE::TEXTURE; }
		inline bool IsBuffer() const { return type == GPU_RESOURCE_TYPE::BUFFER; }

	};

	struct GPUBuffer :public GPUResource
	{

	
		GPUBufferDesc desc;


		 const GPUBufferDesc& GetDesc() const
		{
			return desc;
		}
	};



	struct VertexLayout : public RenderDeviceChild
	{
	
		std::vector<VertexLayoutDesc> desc;
	};

	struct BlendState :public RenderDeviceChild
	{
	
		BlendStateDesc desc;
	    const BlendStateDesc& GetDesc() const
		{
			return desc;
		}

	};

	struct DepthStencilState :public RenderDeviceChild
	{
	
		DepthStencilStateDesc desc;

		const DepthStencilStateDesc& GetDesc() const
		{
			return desc;
		}
	};
	
	struct RasterizerState:public RenderDeviceChild
	{
		RasterizerStateDesc desc;

		const RasterizerStateDesc& GetDesc() const
		{
			return desc;
		}
	};

	struct Texture :public GPUResource
	{

		TextureDesc desc;

		const TextureDesc& GetDesc() const
		{
			return desc;
		}

		

	};


	struct GPUQuery :public RenderDeviceChild
	{
	
		GPUQueryDesc desc;
		const GPUQueryDesc& GetDesc() const {
			return desc;
		}

	};


	struct PipelineState :public RenderDeviceChild
	{
		size_t hash = 0;
		PipelineStateDesc desc;
		const PipelineStateDesc& GetDesc() const { return desc; }

	};

	struct RenderPass :public RenderDeviceChild
	{

		size_t hash = 0;
		RenderPassDesc desc;
		const RenderPassDesc& GetDesc() const
		{
			return desc;
		}

	};
	
}