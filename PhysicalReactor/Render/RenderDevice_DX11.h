#pragma once
#include "CommonInclude.h"
#include "RenderDevice.h"
#include "JobSystem/JobScheduler.h"
#include "JobSystem/ParallelFor.h"
#include "Allocator/Allocator.h"
#include <codecvt>
#include <d3d11_3.h>
#include <DXGI1_3.h>
#include <d3d11_2.h>
#include <DXGI1_2.h>
#include <string.h>
#include <d3d11shader.h>

struct ID3D11Device;
struct ID3D11Device2;

struct IDXGISwapChain1;
struct ID3D11DeviceContext;
struct ID3D11CommandList;
struct ID3DUserDefinedAnnotation;
enum D3D_DRIVER_TYPE;
enum D3D_FEATURE_LEVEL;

struct ID3D11Texture2D;




namespace PRE
{
	class RenderDevice_DX11 :public RenderDevice
	{
	private:
		ID3D11Device* Basedevice = nullptr;
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;
		IDXGISwapChain1* swapChain = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;
		ID3D11DepthStencilView* DepthStecilView = nullptr;
		ID3D11Texture2D* backbuffer = nullptr;
		ID3D11Texture2D* depthbuffer = nullptr;
		ViewPort viewport;
		ID3D11DeviceContext * BasedeviceContexts=nullptr;
		ID3D11CommandList *commandlists[JobScheduler::NumThreads] = {};
		ID3DUserDefinedAnnotation* userDefinedAnnotations[JobScheduler::NumThreads] = {};
		ID3D11Device3* device = nullptr;//
		ID3D11DeviceContext3 *ImmediatedeviceContext = nullptr;
		ID3D11DeviceContext3 *deviceContexts[JobScheduler::NumThreads] = {};//
		UINT stencilRef[JobScheduler::NumThreads];
		XMFLOAT4 blendFactor[JobScheduler::NumThreads];

		ID3D11VertexShader* prev_vs[JobScheduler::NumThreads] = {};
		ID3D11PixelShader* prev_ps[JobScheduler::NumThreads] = {};
		ID3D11HullShader* prev_hs[JobScheduler::NumThreads] = {};
		ID3D11DomainShader* prev_ds[JobScheduler::NumThreads] = {};
		ID3D11GeometryShader* prev_gs[JobScheduler::NumThreads] = {};
		XMFLOAT4 prev_blendfactor[JobScheduler::NumThreads] = {};
		UINT prev_samplemask[JobScheduler::NumThreads] = {};
		ID3D11BlendState* prev_bs[JobScheduler::NumThreads] = {};
		ID3D11RasterizerState* prev_rs[JobScheduler::NumThreads] = {};
		UINT prev_stencilRef[JobScheduler::NumThreads] = {};
		ID3D11DepthStencilState* prev_dss[JobScheduler::NumThreads] = {};
		ID3D11InputLayout* prev_vl[JobScheduler::NumThreads] = {};
		PRIMITIVEOPOLOGY prev_pt[JobScheduler::NumThreads] = {};

		ID3D11UnorderedAccessView* raster_uavs[JobScheduler::NumThreads][9] = {};
		uint8_t raster_uavs_slot[JobScheduler::NumThreads] = {};
		uint8_t raster_uavs_count[JobScheduler::NumThreads] = {};
		void validate_raster_uavs(uint64_t threadID);
		void CreateRenderTargetAndDepthStencil();

	public:
		RenderDevice_DX11(HWND mainscreen,bool fullscreen = false, bool debuglayer = false);
		~RenderDevice_DX11();

		
		


		virtual HRESULT CreateBuffer(const GPUBufferDesc *pDesc, const SubresourceData* pInitialData, GPUBuffer *ppBuffer) override;
		virtual HRESULT CreateTexture1D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture1D **ppTexture1D) override;
		virtual HRESULT CreateTexture2D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture2D **ppTexture2D) override;
		virtual HRESULT CreateTexture3D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture3D **ppTexture3D) override;
		
		virtual HRESULT CreateInputLayout(const VertexLayoutDesc *pInputElementDescs, UINT NumElements, GraphicBlob* blob, VertexLayout *pInputLayout) override;
		virtual HRESULT CreateVertexShader(WCHAR* filename, GraphicBlob* blob, VertexShader* pVertexShader)override;
		virtual HRESULT CreatePixelShader(WCHAR* filename, GraphicBlob* blob, PixelShader* pPixelShader)override;
		virtual HRESULT CreateGemotryShader(WCHAR* filename, GraphicBlob* blob, GeometryShader* pGeometryShader) override;
		virtual HRESULT CreateHullShader(WCHAR* filename, GraphicBlob* blob, HullShader* pHullShader) override;
		virtual HRESULT CreateDomainShader(WCHAR* filename, GraphicBlob* blob, DomainShader* pDomainShader) override;
		virtual HRESULT CreateComputerShader(WCHAR* filename, GraphicBlob* blob, ComputerShader* pComputerShader)override;
		virtual HRESULT CreateBlendState(const BlendStateDesc *pBlendStateDesc, BlendState *pBlendState)override;
		virtual HRESULT CreateDepthStencilState(const DepthStencilStateDesc *pDepthStencilStateDesc, DepthStencilState *pDepthStencilState)override;
		virtual HRESULT CreateRasterizerState(const RasterizerStateDesc *pRasterizerStateDesc, RasterizerState *pRasterizerState)override;
		virtual HRESULT CreateSamplerState(const SamplerDesc *pSamplerDesc, Sampler *pSamplerState)override;
		virtual HRESULT CreateQuery(const GPUQueryDesc *pDesc, GPUQuery *pQuery)override;
		virtual HRESULT CreateGraphicPSO(const GraphicsShaderDesc* pDesc, GraphicPSO* pso)override;
		virtual HRESULT CreateComputerPSO(const ComputerPSODesc* pDesc, ComputerPSO* pso)override;
		////////////
		///////////
		//////////
		virtual void DestroyResource(GPUResource* pResource)override;
		virtual void DestroyBuffer(GPUBuffer* pBuffer)override;
		virtual void DestroyTexture1D(Texture1D* pTexture1D)override;
		virtual void DestroyTexture2D(Texture2D* pTexture2D)override;
		virtual void DestroyTexture3D(Texture3D* pTexture3D)override;
		virtual void DestroyInputLayout(VertexLayout *pInoutLayout)override;
		virtual void DestroyVertexShader(VertexShader* pVertexShader)override;
		virtual void DestroyPixelShader(PixelShader* pPixelShader)override;
		virtual void DestroyGeometryShader(GeometryShader* pGeometryShader)override;
		virtual void DestroyHullShader(HullShader* pHullShader)override;
		virtual void DestroyDomainShader(DomainShader* pDomainShader)override;
		virtual void DestroyComputerShader(ComputerShader* pComputerShader)override;
		virtual void DestroyBlendState(BlendState* pBlendState)override;
		virtual void DestroyDepthStencilState(DepthStencilState* pDepthStencilState)override;
		virtual void DestroyRasterizerState(RasterizerState* pRasterizerstate)override;
		virtual void DestroySamplerState(Sampler* pSamplerState)override;
		virtual void DestroyQuery(GPUQuery *pQuey)override;
		virtual void DestroyGraphicsPSO(GraphicPSO* pso)override;
		virtual void DestroyComputerPSO(ComputerPSO* pso)override;


		virtual void SetName(GPUResource* pResource, const std::string&name)override;
		///////////////////////////////
		virtual void PresentBegin()override;
		virtual void PresentEnd()override;

		virtual void ExcuteDeferredContexts()override;
		virtual void FinishComanlist()override;

		///////////
		///////
		virtual void SetResolution(int width, int Height)override;

		virtual Texture2D GetBackBuffer()override;

		
		virtual void BindScissorRects(UINT numRects, const Rect* rect)override;
		virtual void BindViewports(UINT NumViewports, const ViewPort* pViewports)override;
		virtual void BindRenderTargets(UINT NumViews, Texture2D* const *ppRenderTargets, Texture2D* depthStencilTexture,int arrayIndex = -1)override;
		virtual void ClearRenderTarget(Texture* pTexture, const FLOAT ColorRGBA[4], int arrayIndex = -1)override;
		virtual void ClearDepthStencil(Texture2D* pTexture, UINT ClearFlags, FLOAT Depth, UINT8 Stencil,  int arrayIndex = -1)override;
		virtual void BindResource(SHADERSTAGE stage, GPUResource* resource, int slot, int arrayIndex = -1) override;
		virtual void BindResources(SHADERSTAGE stage, GPUResource *const* resources, int slot, int count)override;
		virtual void BindUAV(SHADERSTAGE stage, GPUResource *resources, int slot,  int arrayIndex = -1)override;
		virtual void BindUAVS(SHADERSTAGE stage, GPUResource* const* resource, int slot, int count)override;
		virtual void UnbindResources(int slot, int num)override;
		virtual void UnbindUAVs(int slot, int num)override;
		virtual void BindSampler(SHADERSTAGE stage, Sampler* sampler,int slot,int count)override;
		virtual void BindConstantBuffer(SHADERSTAGE stage, GPUBuffer* buffer, int slot, const UINT *pFirstConstant, const UINT *pNumberConstant)override;
		virtual void BindVertexBuffers(GPUBuffer* const *vertexBuffers, int slot, int count, const UINT* strides, const UINT*offsets)override;
		virtual void BindIndexBuffer(GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, UINT offset)override;
		virtual void BindStencilRef(UINT value)override;
		virtual void BindBlendFactor(XMFLOAT4 value)override;
		////////////////////////////////////////////////////////////////////////////////////////////
		virtual void BindGraphicsPSO(GraphicPSO* pso)override;
		////////////////////////////////////////////////////////////////////////////////////////////
		virtual void BindRasterizerState(RasterizerState RSstate) override;
		virtual void BindComputerPSO(ComputerPSO* pso)override;
		virtual void Draw(int vertexCount, UINT startVertexLociotn)override;
		virtual void DrawIndexed(UINT Indexcount, UINT startVertexLocation, UINT baseVertexLocation)override;
		virtual void DrawInstanced(int vertexCount, int instanceCount, UINT startVertexLocation, UINT startInstanceLocation)override;
		virtual void DrawIndexedInstanced(int indexCount, int instanceCount, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanced)override;
		virtual void DrawInstancedIndirect(GPUBuffer* args, UINT args_offset)override;
		virtual void Dispatch(UINT threadGruopCountX, UINT threadGruopCountY, UINT threadGruopCountZ)override;
		virtual void DispatchIndirect(GPUBuffer* args, UINT args_offset)override;
		virtual void CopyTexture2D(Texture2D* pDest, Texture2D* pSrc)override;
		virtual void CopyTexture2D_Region(Texture2D* pDest, UINT dstMip, UINT dstX, UINT dstY, Texture2D* pSrc, UINT srcMip, UINT ArraySize)override;
		virtual void MSAAResolve(Texture2D* pDst, Texture2D* pSrc)override;
		virtual void UpdateBuffer(GPUBuffer* buffer, const void* data, int datasize = -1)override;
		virtual void *AllocateFromRingBuffer(GPURingBuffer* buffer, size_t dataSize, UINT& offsetIntoBuffer)override;
		virtual void InvalidateBufferAccess(GPUBuffer* buffer)override;
		virtual bool DownloadResource(GPUBuffer* resurceToDownload, GPUResource* resourceDest, void* dataDest)override;
		virtual void QueryBegin(GPUQuery *query)override;
		virtual void QueryEnd(GPUQuery* query)override;
		virtual bool QueryRead(GPUQuery* query)override;
		virtual void UAVBarrier(GPUResource* const* uavs, UINT NumBarries)override;
		virtual void TransitionBarrier(GPUResource* const* resource, UINT NumBarrires, RESOUCRCE_STATES stateBefore, RESOUCRCE_STATES stateAFTER)override;

		virtual void WaitForGPU()override;
		virtual void EventBegin(const std::string& name)override;
		virtual void EventEnd()override;
		virtual void SetMarker(const std::string &name)override;
		virtual void* GetDevice()override;

		virtual void GenerateMips(GPUResource* SRV)override;
		private:
			HRESULT CreateShaderResourceView(Texture1D* pTexture);
			HRESULT CreateShaderResourceView(Texture2D* pTexture);
			HRESULT CreateShaderResourceView(Texture3D* pTexture);
			HRESULT CreateRenderTargetView(Texture2D* pTexture);
			HRESULT CreateRenderTargetView(Texture3D* pTexture);
			HRESULT CreateDepthStencilView(Texture2D* pTexture);
	};
}