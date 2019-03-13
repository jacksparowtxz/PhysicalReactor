#pragma once
#include"CommonInclude.h"
#include"Render/Renderer.h"

using namespace PRE;

class DepthStancilTarget
{
public:
	DepthStancilTarget();
	~DepthStancilTarget();

	Texture2D* texture;
	Texture2D* Reslovetexture;

	void Initialize(int width, int height, UINT MSAAC);
	void InitializeCube(int size, bool independentFaces = false);
	void Clear(GRAPHICSTHREAD threadID);
	void CopyFrom(const DepthStancilTarget&, GRAPHICSTHREAD threadID);

	Texture2D* GetTexture() const { return texture; }
	Texture2D* GetTextureResolvedMSAA(GRAPHICSTHREAD threadID);
	TextureDesc GetDesc() const { return GetTexture()->GetDesc(); }
};

