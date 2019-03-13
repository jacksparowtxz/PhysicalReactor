#include "stdafx.h"
#include "DepthStancilTarget.h"


DepthStancilTarget::DepthStancilTarget()
{
}


DepthStancilTarget::~DepthStancilTarget()
{
}

void DepthStancilTarget::Initialize(int width, int height, UINT MSAAC)
{
}

void DepthStancilTarget::InitializeCube(int size, bool independentFaces)
{
}

void DepthStancilTarget::Clear(GRAPHICSTHREAD threadID)
{
}

void DepthStancilTarget::CopyFrom(const DepthStancilTarget &, GRAPHICSTHREAD threadID)
{
}

Texture2D * DepthStancilTarget::GetTextureResolvedMSAA(GRAPHICSTHREAD threadID)
{
	return nullptr;
}
