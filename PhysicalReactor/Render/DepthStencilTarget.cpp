#include "stdafx.h"
#include "DepthStencilTarget.h"
#include "Render/ResourceMapping.h"

DepthStencilTarget::DepthStencilTarget()
{
	texture = nullptr;
	Reslovetexture = nullptr;
	resolvedMSAAUptodate = false;
}


DepthStencilTarget::~DepthStencilTarget()
{
	SAFE_DELETE(texture);
	SAFE_DELETE(Reslovetexture);
}

void DepthStencilTarget::Initialize(int width, int height, UINT MSAAC)
{
	SAFE_DELETE(texture);
	SAFE_DELETE(Reslovetexture);

	resolvedMSAAUptodate = false;
	TextureDesc depthstencildesc;
	depthstencildesc.Width = width;
	depthstencildesc.Height = height;
	depthstencildesc.MipLevels = 1;
	depthstencildesc.ArraySize = 1;
	depthstencildesc.Format=PRE::FORMAT_R32G8X24_TYPELESS;
	depthstencildesc.CPUAccessFlags = 0;
	depthstencildesc.MiscFlags = 0;

	Renderer::GetDevice()->CreateTexture2D(&depthstencildesc,nullptr,&texture);

	if (MSAAC > 1)
	{
		depthstencildesc.SampleDesc.Count = 1;
		depthstencildesc.Format = FORMAT_R32_FLOAT;
		depthstencildesc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS;
		Renderer::GetDevice()->CreateTexture2D(&depthstencildesc, nullptr, &Reslovetexture);
	}





}

void DepthStencilTarget::InitializeCube(int size, bool independentFaces)
{
	SAFE_DELETE(texture);
	SAFE_DELETE(Reslovetexture);
	resolvedMSAAUptodate = false;

	TextureDesc depthGPUBufferDesc;
	ZeroMemory(&depthGPUBufferDesc, sizeof(depthGPUBufferDesc));

	depthGPUBufferDesc.Width = size;
	depthGPUBufferDesc.Height = size;
	depthGPUBufferDesc.MipLevels = 1;
	depthGPUBufferDesc.ArraySize = 6;
	depthGPUBufferDesc.Format = PRE::FORMAT_R16_TYPELESS;
	depthGPUBufferDesc.SampleDesc.Count = 1;
	depthGPUBufferDesc.SampleDesc.Quality = 0;
	depthGPUBufferDesc.Usage = USAGE_DEFAULT;
	depthGPUBufferDesc.BindFlags = BIND_DEPTH_STENCIL | BIND_SHADER_RESOURCE;
	depthGPUBufferDesc.CPUAccessFlags = 0;
	depthGPUBufferDesc.MiscFlags = RESOURCE_MISC_TEXTURECUBE;

	texture = new Texture2D;
	texture->RequestIndepentRenderTargetCubemapFaces(independentFaces);
	Renderer::GetDevice()->CreateTexture2D(&depthGPUBufferDesc, nullptr, &texture);

}

void DepthStencilTarget::Clear()
{
	Renderer::GetDevice()->ClearDepthStencil(GetTexture(),CLEAR_DEPTH|CLEAR_STENCIL,0.0f,0);
	resolvedMSAAUptodate = false;
}

void DepthStencilTarget::CopyFrom(const DepthStencilTarget &form)
{
	Renderer::GetDevice()->CopyTexture2D(GetTexture(), form.GetTexture());
}

Texture2D * DepthStencilTarget::GetTextureResolvedMSAA()
{
	if (Reslovetexture != nullptr)
	{
		if (!resolvedMSAAUptodate)
		{
			Renderer::GetDevice()->EventBegin("Resolve MSAA DepthBuffer");
			Renderer::GetDevice()->BindResource(CS_STAGE, Reslovetexture, TEXSLOT_ONDEMAND0);/////////////////////////////////
			Renderer::GetDevice()->BindUAV(CS_STAGE, texture, 0);

			TextureDesc desc = Reslovetexture->GetDesc();
			ComputerPSO computerpso;
			computerpso.desc.cs=Renderer::shadermanager->GetComputerShader("ResloveDepthStencil.hlsl");
			Renderer::GetDevice()->BindComputerPSO(&computerpso);
			Renderer::GetDevice()->Dispatch((UINT)ceilf(desc.Width / 16.f), (UINT)ceilf(desc.Height / 16.f), 1);

			Renderer::GetDevice()->UnbindResources(TEXSLOT_ONDEMAND0, 1);
			Renderer::GetDevice()->UnbindUAVs(0,1);

			Renderer::GetDevice()->EventEnd();

		}
	}
	return texture;
}
