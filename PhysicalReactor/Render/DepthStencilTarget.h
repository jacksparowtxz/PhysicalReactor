#pragma once
#include"CommonInclude.h"
#include"Render/Renderer.h"

using namespace PRE;

class DepthStencilTarget
{
public:
	DepthStencilTarget();
	~DepthStencilTarget();
	bool resolvedMSAAUptodate;
	Texture2D* texture;
	Texture2D* Reslovetexture;

	void Initialize(int width, int height, UINT MSAAC);
	void InitializeCube(int size, bool independentFaces = false);
	void Clear();
	void CopyFrom(const DepthStencilTarget&);

	Texture2D* GetTexture() const { return texture; }
	Texture2D* GetTextureResolvedMSAA();
	TextureDesc GetDesc() const { return GetTexture()->GetDesc(); }
};

