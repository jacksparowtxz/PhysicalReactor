#include "stdafx.h"
#include "RenderTarget.h"


RenderTarget::RenderTarget()
{
}

RenderTarget::RenderTarget(UINT width, UINT height, bool hasdepth, FORMAT format, UINT mipMapLevelCount, UINT MSAAC, bool depthonly)
{
}

void RenderTarget::CleanUp()
{
}

void RenderTarget::Initialize(UINT width, UINT height, bool hasDepth, FORMAT format, UINT mipMapLevelCount, UINT MSAAC, bool depthOnly)
{
}

void RenderTarget::InitializeCube(UINT size, bool hasDepth, FORMAT format, UINT mipMapLevelCount, bool depthOnly)
{
}

void RenderTarget::Add(FORMAT format)
{
}

void RenderTarget::Activate(GRAPHICSTHREAD threadID, bool disableColor, int viewID)
{
}

void RenderTarget::Activate(GRAPHICSTHREAD threadID, float r, float g, float b, float a, bool disableColor, int viewID)
{
}

void RenderTarget::Activate(GRAPHICSTHREAD threadID, DepthStencilTarget *, float r, float g, float b, float a, bool disableColor, int viewID)
{
}

void RenderTarget::Activate(GRAPHICSTHREAD threadID, DepthStencilTarget *, bool disableColor, int viewID)
{
}

void RenderTarget::Deactivate(GRAPHICSTHREAD threadID)
{
}

void RenderTarget::Set(GRAPHICSTHREAD threadID, bool disableColor, int viewID)
{
}

void RenderTarget::Set(GRAPHICSTHREAD threadID, DepthStencilTarget *, bool disableColor, int viewID)
{
}

PRE::Texture2D * RenderTarget::GetTextureResolvedMSAA(GRAPHICSTHREAD threadID, int viewID)
{
	return nullptr;
}

UINT RenderTarget::GetMipCount()
{
	return 0;
}


RenderTarget::~RenderTarget()
{
}
