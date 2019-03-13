#pragma once
#include"CommonInclude.h"
#include"Render/Renderer.h"
#include"Render/GraphicsResource.h"
#include"Container/vector.h"

using namespace PRE;

class DepthStencilTarget;


class RenderTarget
{
public:
	RenderTarget();
	RenderTarget(UINT width, UINT height, bool hasdepth, FORMAT format, UINT mipMapLevelCount, UINT MSAAC, bool depthonly);
	void CleanUp();

	void Initialize(UINT width, UINT height, bool hasDepth = false, FORMAT format = FORMAT_R8G8B8A8_UNORM, UINT mipMapLevelCount = 1, UINT MSAAC = 1, bool depthOnly = false);
	void InitializeCube(UINT size, bool hasDepth, FORMAT format = FORMAT_R8G8B8A8_UNORM, UINT mipMapLevelCount = 1, bool depthOnly = false);
	void Add(FORMAT format =FORMAT_R8G8B8A8_UNORM);
	void Activate(GRAPHICSTHREAD threadID, bool disableColor = false, int viewID = -1);
	void Activate(GRAPHICSTHREAD threadID, float r, float g, float b, float a, bool disableColor = false, int viewID = -1);
	void Activate(GRAPHICSTHREAD threadID, DepthStencilTarget*, float r, float g, float b, float a, bool disableColor = false, int viewID = -1);
	void Activate(GRAPHICSTHREAD threadID, DepthStencilTarget*, bool disableColor = false, int viewID = -1);
	void Deactivate(GRAPHICSTHREAD threadID);
	void Set(GRAPHICSTHREAD threadID, bool disableColor = false, int viewID = -1);
	void Set(GRAPHICSTHREAD threadID, DepthStencilTarget*, bool disableColor = false, int viewID = -1);

	PRE::Texture2D* GetTexture(int viewID = 0) const { return rendertargets[viewID]; }
	PRE::Texture2D* GetTextureResolvedMSAA(GRAPHICSTHREAD threadID, int viewID = 0);
	PRE::TextureDesc GetDesc(int viewID = 0) const { assert(viewID < numviews); return GetTexture(viewID)->GetDesc(); }
	UINT GetMipCount();
	bool IsInitialized() const { return (numviews > 0 || depthstencil != nullptr); }

	~RenderTarget();
	ViewPort viewprot;
	DepthStencilTarget* depthstencil;


private:

	int numviews;
	vector<Texture2D*> rendertargets;
	vector<Texture2D*> resolve_rendertargets;



};

