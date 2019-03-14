#include "stdafx.h"
#include "RenderTarget.h"
#include"Render/DepthStencilTarget.h"
using namespace PRE;

RenderTarget::RenderTarget()
{
	numviews = 0;
	depthstencil = nullptr;
}

RenderTarget::RenderTarget(UINT width, UINT height, bool hasdepth, FORMAT format, UINT mipMapLevelCount, UINT MSAAC, bool depthonly)
{
	numviews = 0;
	depthstencil = nullptr;
	Initialize(width, height, hasdepth, format, mipMapLevelCount, MSAAC,depthonly);
}

void RenderTarget::CleanUp()
{
	for (size_t i = 0; i < rendertargets.size(); ++i)
	{
		SAFE_DELETE(rendertargets[i]);
	}
	for (size_t i = 0; i < resolve_rendertargets.size(); ++i)
	{
		SAFE_DELETE(resolve_rendertargets[i]);
	}
	rendertargets.clear();
	resolve_rendertargets.clear();
	SAFE_DELETE(depthstencil);
	resolvedMSAAUptodate.clear();
}

void RenderTarget::Initialize(UINT width, UINT height, bool hasDepth, FORMAT format, UINT mipMapLevelCount, UINT MSAAC, bool depthOnly)
{
	CleanUp();

	if (!depthOnly)
	{
		TextureDesc texturedesc;
		ZeroMemory(&texturedesc, sizeof(texturedesc));
		texturedesc.Width = width;
		texturedesc.Height = height;
		texturedesc.MipLevels = mipMapLevelCount;
		texturedesc.ArraySize = 1;
		texturedesc.Format = format;
		texturedesc.SampleDesc.Count = MSAAC;
		texturedesc.Usage = USAGE_DEFAULT;
		texturedesc.BindFlags = BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
		texturedesc.CPUAccessFlags = 0;
		texturedesc.MiscFlags = 0;

		numviews = 1;
		Texture2D* texture = new Texture2D;
		if (mipMapLevelCount != 1)
		{
			texture->RequestIndepentShaderReourcesForMIPs(true);
			texture->RequesIndenpentUnorderedAccessResoucesForMips(true);
			texturedesc.BindFlags |= BIND_UNORDERED_ACCESS;
		}

		rendertargets.push_back(texture);
		Renderer::GetDevice()->CreateTexture2D(&texturedesc,nullptr,&rendertargets[0]);
		if (MSAAC > 1)
		{
			texturedesc.SampleDesc.Count = 1;
			resolve_rendertargets.push_back(nullptr);
			Renderer::GetDevice()->CreateTexture2D(&texturedesc, nullptr, &resolve_rendertargets[0]);
			resolvedMSAAUptodate.push_back(false);
		}
		else
		{
			resolvedMSAAUptodate.push_back(true);
		}
	}

	viewprot.Width = (float)width;
	viewprot.Height = (float)height;
	viewprot.MinDepth = 0.0f;
	viewprot.MaxDepth = 1.0f;
	viewprot.TopLeftX = 0;
	viewprot.TopLeftY = 0;

	if (hasDepth)
	{
		depthstencil = new DepthStencilTarget();
	}
}

void RenderTarget::InitializeCube(UINT size, bool hasDepth, FORMAT format, UINT mipMapLevelCount, bool depthOnly)
{
	CleanUp();

	if (!depthOnly)
	{
		TextureDesc texturedesc;
		ZeroMemory(&texturedesc, sizeof(texturedesc));
		texturedesc.Width = size;
		texturedesc.Height = size;
		texturedesc.MipLevels = mipMapLevelCount;
		texturedesc.ArraySize = 6;
		texturedesc.Format = format;
		texturedesc.SampleDesc.Count = 1;
		texturedesc.SampleDesc.Quality = 0;
		texturedesc.Usage = USAGE_DEFAULT;
		texturedesc.BindFlags = BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
		texturedesc.CPUAccessFlags = 0;
		texturedesc.MiscFlags = RESOURCE_MISC_TEXTURECUBE;

		Texture2D* texture = new Texture2D;
		if (mipMapLevelCount != 1)
		{
			texture->RequestIndepentShaderReourcesForMIPs(true);
			texture->RequesIndenpentUnorderedAccessResoucesForMips(true);
			texturedesc.BindFlags |= BIND_UNORDERED_ACCESS;
		}
		
		numviews = 1;
		rendertargets.push_back(texture);
		Renderer::GetDevice()->CreateTexture2D(&texturedesc, nullptr, &rendertargets[0]);
		resolvedMSAAUptodate.push_back(true);
	}

	viewprot.Width = (float)size;
	viewprot.Height = (float)size;
	viewprot.MinDepth = 0.0f;
	viewprot.MaxDepth = 1.0f;
	viewprot.TopLeftX = 0;
	viewprot.TopLeftY = 0;

	if (hasDepth)
	{
		depthstencil = new DepthStencilTarget();
		depthstencil->InitializeCube(size);
	}
}

void RenderTarget::Add(FORMAT format)
{
	TextureDesc desc = GetTexture(0)->GetDesc();
	desc.Format = format;

	if (!rendertargets.empty())
	{
		numviews++;
		rendertargets.push_back(nullptr);
		Renderer::GetDevice()->CreateTexture2D(&desc, nullptr, &rendertargets.back());
		if (desc.SampleDesc.Count > 1)
		{
			desc.SampleDesc.Count = 1;
			resolve_rendertargets.push_back(nullptr);
			Renderer::GetDevice()->CreateTexture2D(&desc, nullptr, &resolve_rendertargets.back());
			resolvedMSAAUptodate.push_back(false);
		}
		else
		{
			resolvedMSAAUptodate.push_back(true);
		}
	}
	else
	{
		assert(0);
	}
}

void RenderTarget::Activate(bool disableColor, int viewID)
{
	Activate(0, 0, 0, 0, disableColor, viewID);
}

void RenderTarget::Activate(float r, float g, float b, float a, bool disableColor, int viewID)
{
	Set(disableColor,viewID);
	float ClearColor[4] = { r,g,b,a };
	if (viewID >= 0)
	{
		Renderer::GetDevice()->ClearRenderTarget(GetTexture(viewID), ClearColor);
	}
	else
	{
		for (int i = 0; i < numviews; ++i)
		{
			Renderer::GetDevice()->ClearRenderTarget(GetTexture(i), ClearColor);
		}
		
	}
	if (depthstencil)
		depthstencil->Clear();
}

void RenderTarget::Activate(DepthStencilTarget *getDepth, float r, float g, float b, float a, bool disableColor, int viewID)
{
	Set(disableColor,viewID);
	float ClearColor[4] = {r,g,b,a};
	if (viewID >= 0)
	{
		Renderer::GetDevice()->ClearRenderTarget(GetTexture(viewID),ClearColor);
	}
	else
	{
		for (int i = 0; i < numviews; i++)
		{
			Renderer::GetDevice()->ClearRenderTarget(GetTexture(i), ClearColor);
		}
	}
}

void RenderTarget::Activate(DepthStencilTarget *getDepth, bool disableColor, int viewID)
{
	Activate(getDepth, 0, 0, 0, 0, disableColor, viewID);
}

void RenderTarget::Deactivate()
{
	Renderer::GetDevice()->BindRenderTargets(0, nullptr, nullptr);
}

void RenderTarget::Set(bool disableColor, int viewID)
{
	Renderer::GetDevice()->BindViewports(1, &viewprot);
	if (viewID >= 0)
	{
		Renderer::GetDevice()->BindRenderTargets(disableColor?0:1,disableColor?nullptr:(Texture2D**)&rendertargets[viewID],(depthstencil?depthstencil->GetTexture():nullptr));
	}
	else
	{
		Renderer::GetDevice()->BindRenderTargets(disableColor ? 0 : numviews, disableColor ? nullptr : (Texture2D**)rendertargets.data(), (depthstencil ? depthstencil->GetTexture() : nullptr));
		for (auto& x : resolvedMSAAUptodate)
		{
			x = false;
		}
	}
}

void RenderTarget::Set(DepthStencilTarget *getDepth, bool disableColor, int viewID)
{
	Renderer::GetDevice()->BindViewports(1, &viewprot);
	if (viewID >= 0)
	{
		Renderer::GetDevice()->BindRenderTargets(disableColor ? 0 : 1, disableColor ? nullptr : (Texture2D**)&rendertargets[viewID], (getDepth ? getDepth->GetTexture() : nullptr));
		resolvedMSAAUptodate[viewID] = false;
	}
	else
	{
		Renderer::GetDevice()->BindRenderTargets(disableColor ? 0 : numviews, disableColor ? nullptr : (Texture2D**)rendertargets.data(), (getDepth ? getDepth->GetTexture() : nullptr));
		for (auto& x : resolvedMSAAUptodate)
		{
			x = false;
		}
	}

}

PRE::Texture2D * RenderTarget::GetTextureResolvedMSAA( int viewID)
{
	if (GetDesc(viewID).SampleDesc.Count > 1)
	{
		Renderer::GetDevice()->MSAAResolve(resolve_rendertargets[viewID], rendertargets[viewID]);
		resolvedMSAAUptodate[viewID] = true;
	}
	return resolve_rendertargets[viewID];
}

UINT RenderTarget::GetMipCount()
{
	TextureDesc desc = GetDesc();

	if (desc.MipLevels > 0)
	{
		return desc.MipLevels;
	}

	UINT maxDim = max(desc.Width, desc.Height);
	return static_cast<UINT>(log2(static_cast<double>(maxDim)));
}


RenderTarget::~RenderTarget()
{
	CleanUp();
}
