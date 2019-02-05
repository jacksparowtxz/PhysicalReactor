#include "GraphicsResource.h"
#include "RenderDevice.h"


namespace PRE
{

	VertexShader::VertexShader()
	{
	
	}

	VertexShader::~VertexShader()
	{

		if (device != nullptr)
		{
			device->DestroyVertexShader(this);
		}
	}

	PixelShader::PixelShader()
	{
		
	}

	PixelShader::~PixelShader()
	{

		if (device != nullptr)
		{
			device->DestroyPixelShader(this);
		}
	}

	GeometryShader::GeometryShader()
	{
	
	}
	GeometryShader::~GeometryShader()
	{
		if (device != nullptr)
		{
			device->DestroyGeometryShader(this);
		}
	}

	DomainShader::DomainShader()
	{
		
	}
	DomainShader::~DomainShader()
	{
		if (device != nullptr)
		{
			device->DestroyDomainShader(this);
		}
	}

	HullShader::HullShader()
	{
		
	}
	HullShader::~HullShader()
	{
		if (device != nullptr)
		{
			device->DestroyHullShader(this);
		}
	}

	ComputerShader::ComputerShader()
	{
	
		
	}
	ComputerShader::~ComputerShader()
	{
		if (device != nullptr)
		{
			device->DestroyComputerShader(this);
		}
	}

	Sampler::Sampler()
	{
		
	}

	Sampler::~Sampler()
	{
		if (device != nullptr)
		{
			device->DestroySamplerState(this);
		}
	}

	GPUResource::GPUResource()
	{
		
	}

	GPUResource::~GPUResource()
	{
		if (device != nullptr)
		{
			device->DestroyResource(this);
		}
	}

	GPUBuffer::GPUBuffer() :GPUResource()
	{
		
	}

	GPUBuffer::~GPUBuffer()
	{
		if (device != nullptr)
		{
			device->DestroyBuffer(this);
		}
	}

	VertexLayout::VertexLayout()
	{
		
	}

	VertexLayout::~VertexLayout()
	{
		if (device != nullptr)
		{
			device->DestroyInputLayout(this);
		}
	}

	BlendState::BlendState()
	{
		
	}

	BlendState::~BlendState()
	{
		if (device != nullptr)
		{
			device->DestroyBlendState(this);
		}
	}

	DepthStencilState::DepthStencilState()
	{
		
	}

	DepthStencilState::~DepthStencilState()
	{
		if (device != nullptr)
		{
			device->DestroyDepthStencilState(this);
		}
	}

	RasterizerState::RasterizerState()
	{
		
	}
	RasterizerState::~RasterizerState()
	{
		if (device != nullptr)
		{
			device->DestroyRasterizerState(this);
		}
	}

	Texture::Texture() :GPUResource()
	{
	
	}
	Texture::~Texture()
	{

	}

	void Texture::RequestIndependentRenderTargetArraySlices(bool value)
	{
		independentRTVArraySlices = value;
	}

	void Texture::RequestIndepentShaderReourcesForMIPs(bool value)
	{
		indepentdentSRVMIPs = value;
	}

	void Texture::RequestIndepentRenderTargetCubemapFaces(bool value)
	{
		independentRTVCubemapFaces = value;
	}

	void Texture::RequestIndepentShaderResourceArraySlices(bool value)
	{
		independentSRVArraySlices = value;
	}

	void Texture::RequesIndenpentUnorderedAccessResoucesForMips(bool value)
	{
		indepentdentUAVMIPs = value;
	}


	Texture1D::Texture1D() : Texture()
	{

	}

	Texture1D::~Texture1D()
	{
		if (device != nullptr)
		{
			device->DestroyTexture1D(this);
		}
	}

	Texture2D::Texture2D() : Texture()
	{
	
	}

	Texture2D::~Texture2D()
	{
		if (device != nullptr)
		{
			device->DestroyTexture2D(this);
		}
	}

	Texture3D::Texture3D() : Texture()
	{
		
	}

	Texture3D::~Texture3D()
	{
		if (device != nullptr)
		{
			device->DestroyTexture3D(this);
		}
	}

	GPUQuery::GPUQuery()
	{
		async_frameshift = 0;
	}

	GPUQuery::~GPUQuery()
	{
		if (device != nullptr)
		{
			device->DestroyQuery(this);
		}
	}


	GraphicPSO::GraphicPSO()
	{
		
	}
	GraphicPSO::~GraphicPSO()
	{
	    if (device!=nullptr)
	    {
			device->DestroyGraphicsPSO(this);
	    }
	}

	ComputerPSO::ComputerPSO()
	{
	
	}

	ComputerPSO::~ComputerPSO()
	{
	     if (device!=nullptr)
	     {
			 device->DestroyComputerPSO(this);
	     }
	}

	GraphicBlob::GraphicBlob()
	{
		
	}

	GraphicBlob::~GraphicBlob()
	{
		
	}
	ConstantBufferLayoutDX::ConstantBufferLayoutDX() 
	{

	}

	ShaderReflectionDX::ShaderReflectionDX() 
	{

	}

	ShaderReflection::ShaderReflection()
	{
		stage = STAGE_COUNT;
	}

	ShaderReflection::~ShaderReflection()
	{

	}

}