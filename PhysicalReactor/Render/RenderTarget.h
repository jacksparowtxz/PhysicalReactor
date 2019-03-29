#pragma once
#include"CommonInclude.h"
#include"Render/Renderer.h"
#include"Render/GraphicsResource.h"
#include<vector>
using namespace std;
using namespace PRE;

class DepthStencilTarget;

namespace PRE
{
	class RenderTarget
	{
	public:
		RenderTarget();
		RenderTarget(UINT width, UINT height, bool hasdepth, FORMAT format, UINT mipMapLevelCount, UINT MSAAC, UINT MSAAQUALITY,bool depthonly);
		void CleanUp();

		void Initialize(UINT width, UINT height, bool hasDepth = false, FORMAT format = FORMAT_R8G8B8A8_UNORM, UINT mipMapLevelCount = 1, UINT MSAAC = 1, UINT MSAAQUALITY = 0, bool depthOnly = false);
		void InitializeCube(UINT size, bool hasDepth, FORMAT format = FORMAT_R8G8B8A8_UNORM, UINT mipMapLevelCount = 1, bool depthOnly = false);
		void Add(FORMAT format = FORMAT_R8G8B8A8_UNORM);
		void Activate(bool disableColor = false, int viewID = -1);
		void Activate(float r, float g, float b, float a, bool disableColor = false, int viewID = -1);
		void Activate(DepthStencilTarget*, float r, float g, float b, float a, bool disableColor = false, int viewID = -1);
		void Activate(DepthStencilTarget*, bool disableColor = false, int viewID = -1);
		void Clear_Immediate(bool disableColor = false, int viewID = -1);
		void Clear_Immediate(float r, float g, float b, float a, bool disableColor = false, int viewID = -1);
		void Clear_Immediate(DepthStencilTarget*, float r, float g, float b, float a, bool disableColor = false, int viewID = -1);
		void Clear_Immediate(DepthStencilTarget*, bool disableColor = false, int viewID = -1);
		void Deactivate();
		void Set(bool disableColor = false, int viewID = -1);
		void Set(DepthStencilTarget*, bool disableColor = false, int viewID = -1);
		void ConvertoResolve();
		PRE::Texture2D* GetTexture(int viewID = 0) const { return rendertargets[viewID]; }
		PRE::Texture2D* GetTextureResolvedMSAA(int viewID = 0);
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
		vector<int>	resolvedMSAAUptodate;


	};
}



