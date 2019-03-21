#pragma once
#include "CommonInclude.h"
#include "GraphicsDescriptors.h"
#include "GraphicsResource.h"
#include "GraphicEnum.h"
#include "JobSystem/ParallelFor.h"
#include "Allocator/Allocator.h"
#include <memory>
#include <winerror.h>

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

namespace PRE
{
	class RenderDevice
	{
	protected:
		uint64_t FRAMECOUNT;
		bool VSYNC;
		int SCREENWIDTH;
		int SCREENHEIGHT;
		bool FULLSCREEN;
		bool RESOLUTIONCHANGED;
		FORMAT BACKBUFFER_FORMAT;
		static const UINT BACKBUFFER_COUNT = 2;
		bool TESSELLATION;
		bool MULTITHREAD_RENDERING;
		bool CONSERVATIVE_RASTERIZATION;
		bool RASTERIZER_ORDERED_VIEWS;
		bool UNORDEREDACCESSTEXTURE_LOAD_EXT;
		UINT M4XMSAAQUALITY;
	public:
		RenderDevice() :FRAMECOUNT(0), VSYNC(true), SCREENHEIGHT(0), FULLSCREEN(false), RESOLUTIONCHANGED(false),
			BACKBUFFER_FORMAT(FORMAT_R16G16B16A16_FLOAT), TESSELLATION(false), MULTITHREAD_RENDERING(false), CONSERVATIVE_RASTERIZATION(false), RASTERIZER_ORDERED_VIEWS(false), UNORDEREDACCESSTEXTURE_LOAD_EXT(false),M4XMSAAQUALITY(0)
		{
		
		}

		///////////////
		//////////
		//////////////
		virtual ~RenderDevice()=0;
		virtual HRESULT CreateBuffer(const GPUBufferDesc* pDesc, const SubresourceData* pInitialData, GPUBuffer* ppBuffer) = 0;
		virtual HRESULT CreateTexture1D(const TextureDesc* pDesc, const SubresourceData* pInitialData, Texture1D**ppTexture1D) = 0;
		virtual HRESULT CreateTexture2D(const TextureDesc* pDesc, const SubresourceData* pInitialData, Texture2D**ppTexture2D) = 0;
		virtual HRESULT CreateTexture3D(const TextureDesc* pDesc, const SubresourceData* pInitialData, Texture3D**ppTexture3D) = 0;

		virtual HRESULT CreateInputLayout(const VertexLayoutDesc *pInputElementDescs, UINT NumElements, GraphicBlob* blob, VertexLayout *pInputLayout) = 0;
		virtual HRESULT CreateVertexShader(WCHAR* filename,GraphicBlob* blob, VertexShader* pVertexShader) = 0;
		virtual HRESULT CreatePixelShader(WCHAR* filename, GraphicBlob* blob,PixelShader* pPixelShader) = 0;
		virtual HRESULT CreateGemotryShader(WCHAR* filename, GraphicBlob* blob, GeometryShader* pGeometryShader) = 0;
		virtual HRESULT CreateHullShader(WCHAR* filename, GraphicBlob* blob, HullShader* pHullShader) = 0;
		virtual HRESULT CreateDomainShader(WCHAR* filename, GraphicBlob* blob, DomainShader* pDomainShader) = 0;
		virtual HRESULT CreateComputerShader(WCHAR* filename, GraphicBlob* blob, ComputerShader* pComputerShader) = 0;
		virtual HRESULT CreateBlendState(const BlendStateDesc* pBlendStateDesc, BlendState *pBlendState) = 0;
		virtual HRESULT CreateDepthStencilState(const DepthStencilStateDesc* pDepthStencilStatedesc, DepthStencilState* pDepthStencilState) = 0;
		virtual HRESULT CreateRasterizerState(const RasterizerStateDesc* pRasterizerStateDesc, RasterizerState* pDepthStencilState) = 0;
		virtual HRESULT CreateSamplerState(const SamplerDesc *pSamplerDesc, Sampler *pSamplerState) = 0;
		virtual HRESULT CreateQuery(const GPUQueryDesc* pDesc, GPUQuery *pQuery) = 0;
		virtual HRESULT CreateGraphicPSO(const GraphicsShaderDesc* pDesc, GraphicPSO* pso) = 0;
		virtual HRESULT CreateComputerPSO(const ComputerPSODesc* pDesc, ComputerPSO* pso) = 0;
	
		

		virtual void DestroyResource(GPUResource* pResource) = 0;
		virtual void DestroyBuffer(GPUBuffer* pBuffer) = 0;
		virtual void DestroyTexture1D(Texture1D* pTexture1D) = 0;
		virtual void DestroyTexture2D(Texture2D* pTexture2D) = 0;
		virtual void DestroyTexture3D(Texture3D* pTexture3D) = 0;
		virtual void DestroyInputLayout(VertexLayout *pInoutLayout) = 0;
		virtual void DestroyVertexShader(VertexShader* pVertexShader) = 0;
		virtual void DestroyPixelShader(PixelShader* pPixelShader) = 0;
		virtual void DestroyGeometryShader(GeometryShader* pGeometryShader) = 0;
		virtual void DestroyHullShader(HullShader* pHullShader) = 0;
		virtual void DestroyDomainShader(DomainShader* pDomainShader) = 0;
		virtual void DestroyComputerShader(ComputerShader* pComputerShader) = 0;
		virtual void DestroyBlendState(BlendState* pBlendState) = 0;
		virtual void DestroyDepthStencilState(DepthStencilState* pDepthStencilState) = 0;
		virtual void DestroyRasterizerState(RasterizerState* pRasterizerstate)=0;
		virtual void DestroySamplerState(Sampler* pSamplerState) = 0;
		virtual void DestroyQuery(GPUQuery *pQuey) = 0;
		virtual void DestroyGraphicsPSO(GraphicPSO* pso) = 0;
		virtual void DestroyComputerPSO(ComputerPSO* pso) = 0;
		/////////////////////////

		virtual void SetName(GPUResource* pResource, const std::string&name) = 0;
		///////////////////////////////
		virtual void PresentBegin() = 0;
		virtual void PresentEnd() = 0;

		virtual void ExcuteDeferredContexts() = 0;
		virtual void FinishComanlist() = 0;

		bool GetVSyncEnable()
		{
			return VSYNC;
		}

		void SetVSyncEnable(bool value)
		{
			VSYNC = value;
		}

		uint64_t GetFrameCount()
		{
			return FRAMECOUNT;
		}

		int GetScreenWidth()
		{
			return SCREENWIDTH;
		}

		int GetScreenHeight()
		{
			return SCREENHEIGHT;
		}

		bool ResolutionChanged()
		{
		
			return RESOLUTIONCHANGED;
		}

		int GetMSAAQUALITY()
		{
			return M4XMSAAQUALITY;
		}
		virtual void SetResolution(int width, int Height) = 0;

		virtual Texture2D GetBackBuffer() = 0;

		enum GRAPHICSDEVICE_CAPABILITY
		{
			GRAPHICSDEVICE_CAPABILITY_TESSELLATION,
			GRAPHICSDEVICE_CAPABILITY_MULTITHREAD_RENDERING,
			GRAPHICSDEVICE_CAPABILITY_CONSERVATIVE_RASTERIZATION,
			GRAPHICSDEVICE_CAPABILITY_RASTERIZER_ORDERED_VIEWS,
			GRAPHICSDEVICE_CAPABILITY_UNORDEREDACCESSTEXTURE_LOAD_FORMAT_EXT,
		};

		bool CheckCapability(GRAPHICSDEVICE_CAPABILITY capability);

		uint32_t GetFormatStride(FORMAT value);

		XMMATRIX GetScreenProjection()
		{
			return XMMatrixOrthographicOffCenterLH(0, (float)GetScreenWidth(), (float)GetScreenHeight(), 0, -1, 1);
		}
		FORMAT GetBackBufferFormat()
		{
			return BACKBUFFER_FORMAT;
		}
		static UINT GetBackBufferCount()
		{
			return BACKBUFFER_COUNT;
		}
		
	

		virtual void BindScissorRects(UINT numRects, const Rect* rect) = 0;
		virtual void BindViewports(UINT NumViewports, const ViewPort* pViewports) = 0;
		virtual void BindRenderTargets(UINT NumViews, Texture2D* const *ppRenderTargets, Texture2D* depthStencilTexture, int arrayIndex = -1)=0;
		virtual void ClearRenderTarget(Texture* pTexture, const FLOAT ColorRGBA[4], int arrayIndex = -1) = 0;
		virtual void ClearDepthStencil(Texture2D* pTexture, UINT ClearFlags, FLOAT Depth, UINT8 Stencil, int arrayIndex=-1) = 0;
		virtual void BindResource(SHADERSTAGE stage, GPUResource* resource, int slot, int arrayIndex = -1) = 0;
		virtual void BindResources(SHADERSTAGE stage, GPUResource *const* resources, int slot, int count) = 0;
		virtual void BindUAV(SHADERSTAGE stage, GPUResource *resources, int slot, int arrayIndex = -1) = 0;
		virtual void BindUAVS(SHADERSTAGE stage, GPUResource* const* resource, int slot, int count) = 0;
		virtual void UnbindResources(int slot, int num) = 0;
		virtual void UnbindUAVs(int slot, int num) = 0;
		virtual void BindSampler(SHADERSTAGE stage, Sampler* sampler,  int slot, int count) = 0;
		virtual void BindConstantBuffer(SHADERSTAGE stage, GPUBuffer* buffer, int slot , const UINT *pFirstConstant,const UINT *pNumberConstant) = 0;
		virtual void BindVertexBuffers(GPUBuffer* const *vertexBuffers, int slot, int count, const UINT* strides, const UINT*offsets) = 0;
		virtual void BindIndexBuffer(GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, UINT offset) = 0;
		virtual void BindStencilRef(UINT value) = 0;
		virtual void BindBlendFactor(XMFLOAT4 value) = 0;
		///////////////////////////////////////////////////////////////////////////////////
		virtual void BindGraphicsPSO(GraphicPSO* pso)=0;
		//////////////////////////////////////////////////////////////////////////////////
		virtual void BindRasterizerState(RasterizerState RSstate)=0;
		virtual void BindComputerPSO(ComputerPSO* pso) = 0;
		virtual void Draw(int vertexCount, UINT startVertexLociotn) = 0;
		virtual void DrawIndexed(UINT Indexcount, UINT startVertexLocation,UINT baseVertexLocation) = 0;
		virtual void DrawInstanced(int vertexCount, int instanceCount, UINT startVertexLocation, UINT startInstanceLocation) = 0;
		virtual void DrawIndexedInstanced(int indexCount, int instanceCount, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanced) = 0;
		virtual void DrawInstancedIndirect(GPUBuffer* args, UINT args_offset) = 0;
		virtual void Dispatch(UINT threadGruopCountX, UINT threadGruopCountY, UINT threadGruopCountZ) = 0;
		virtual void DispatchIndirect(GPUBuffer* args, UINT args_offset) = 0;
		virtual void CopyTexture2D(Texture2D* pDest, Texture2D* pSrc) = 0;
		virtual void CopyTexture2D_Region(Texture2D* pDest, UINT dstMip, UINT dstX, UINT dstY, Texture2D* pSrc, UINT srcMip, UINT ArraySize) = 0;
		virtual void MSAAResolve(Texture2D* pDst, Texture2D* pSrc) = 0;
		virtual void UpdateBuffer(GPUBuffer* buffer, const void* data,int datasize=-1) = 0;
		virtual void *AllocateFromRingBuffer(GPURingBuffer* buffer, size_t dataSize, UINT& offsetIntoBuffer) = 0;
		virtual void InvalidateBufferAccess(GPUBuffer* buffer) = 0;
		virtual bool DownloadResource(GPUBuffer* resurceToDownload, GPUResource* resourceDest, void* dataDest) = 0;
		virtual void QueryBegin(GPUQuery *query) = 0;
		virtual void QueryEnd(GPUQuery* query) = 0;
		virtual bool QueryRead(GPUQuery* query) = 0;
		virtual void UAVBarrier(GPUResource* const* uavs, UINT NumBarries) = 0;
		virtual void TransitionBarrier(GPUResource* const* resource, UINT NumBarrires, RESOUCRCE_STATES stateBefore, RESOUCRCE_STATES stateAFTER) = 0;

		virtual void WaitForGPU() = 0;
		virtual void EventBegin(const std::string& name) = 0;
		virtual void EventEnd() = 0;
		virtual void SetMarker(const std::string &name) = 0;
		virtual void GenerateMips(GPUResource* ShaderResourceView)=0;

		///////////////////////////////////////////////Immediate
		virtual void BindScissorRects_Immediate(UINT numRects, const Rect* rect) = 0;
		virtual void BindViewports_Immediate(UINT NumViewports, const ViewPort* pViewports) = 0;
		virtual void BindRenderTargets_Immediate(UINT NumViews, Texture2D* const *ppRenderTargets, Texture2D* depthStencilTexture, int arrayIndex = -1) = 0;
		virtual void ClearRenderTarget_Immediate(Texture* pTexture, const FLOAT ColorRGBA[4], int arrayIndex = -1) = 0;
		virtual void ClearDepthStencil_Immediate(Texture2D* pTexture, UINT ClearFlags, FLOAT Depth, UINT8 Stencil, int arrayIndex = -1) = 0;
		virtual void BindResource_Immediate(SHADERSTAGE stage, GPUResource* resource, int slot, int arrayIndex = -1) = 0;
		virtual void BindResources_Immediate(SHADERSTAGE stage, GPUResource *const* resources, int slot, int count) = 0;
		virtual void BindUAV_Immediate(SHADERSTAGE stage, GPUResource *resources, int slot, int arrayIndex = -1) = 0;
		virtual void BindUAVS_Immediate(SHADERSTAGE stage, GPUResource* const* resource, int slot, int count) = 0;
		virtual void UnbindResources_Immediate(int slot, int num) = 0;
		virtual void UnbindUAVs_Immediate(int slot, int num) = 0;
		virtual void BindSampler_Immediate(SHADERSTAGE stage, Sampler* sampler, int slot, int count) = 0;
		virtual void BindConstantBuffer_Immediate(SHADERSTAGE stage, GPUBuffer* buffer, int slot, const UINT *pFirstConstant, const UINT *pNumberConstant) = 0;
		virtual void BindVertexBuffers_Immediate(GPUBuffer* const *vertexBuffers, int slot, int count, const UINT* strides, const UINT*offsets) = 0;
		virtual void BindIndexBuffer_Immediate(GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, UINT offset) = 0;
		virtual void BindStencilRef_Immediate(UINT value) = 0;
		virtual void BindBlendFactor_Immediate(XMFLOAT4 value) = 0;
		///////////////////////////////////////////////////////////////////////////////////
		virtual void BindGraphicsPSO_Immediate(GraphicPSO* pso) = 0;
		//////////////////////////////////////////////////////////////////////////////////
		virtual void BindRasterizerState_Immediate(RasterizerState RSstate) = 0;
		virtual void BindComputerPSO_Immediate(ComputerPSO* pso) = 0;
		virtual void Draw_Immediate(int vertexCount, UINT startVertexLociotn) = 0;
		virtual void DrawIndexed_Immediate(UINT Indexcount, UINT startVertexLocation, UINT baseVertexLocation) = 0;
		virtual void DrawInstanced_Immediate(int vertexCount, int instanceCount, UINT startVertexLocation, UINT startInstanceLocation) = 0;
		virtual void DrawIndexedInstanced_Immediate(int indexCount, int instanceCount, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanced) = 0;
		virtual void DrawInstancedIndirect_Immediate(GPUBuffer* args, UINT args_offset) = 0;
		virtual void Dispatch_Immediate(UINT threadGruopCountX, UINT threadGruopCountY, UINT threadGruopCountZ) = 0;
		virtual void DispatchIndirect_Immediate(GPUBuffer* args, UINT args_offset) = 0;
		virtual void CopyTexture2D_Immediate(Texture2D* pDest, Texture2D* pSrc) = 0;
		virtual void CopyTexture2D_Region_Immediate(Texture2D* pDest, UINT dstMip, UINT dstX, UINT dstY, Texture2D* pSrc, UINT srcMip, UINT ArraySize) = 0;
		virtual void MSAAResolve_Immediate(Texture2D* pDst, Texture2D* pSrc) = 0;
		virtual void UpdateBuffer_Immediate(GPUBuffer* buffer, const void* data, int datasize = -1) = 0;
		virtual void *AllocateFromRingBuffer_Immediate(GPURingBuffer* buffer, size_t dataSize, UINT& offsetIntoBuffer) = 0;
		virtual void InvalidateBufferAccess_Immediate(GPUBuffer* buffer) = 0;
		virtual bool DownloadResource_Immediate(GPUBuffer* resurceToDownload, GPUResource* resourceDest, void* dataDest) = 0;
		virtual void QueryBegin_Immediate(GPUQuery *query) = 0;
		virtual void QueryEnd_Immediate(GPUQuery* query) = 0;
		virtual bool QueryRead_Immediate(GPUQuery* query) = 0;
		virtual void UAVBarrier_Immediate(GPUResource* const* uavs, UINT NumBarries) = 0;
		virtual void TransitionBarrier_Immediate(GPUResource* const* resource, UINT NumBarrires, RESOUCRCE_STATES stateBefore, RESOUCRCE_STATES stateAFTER) = 0;

		virtual void WaitForGPU_Immediate() = 0;
		virtual void EventBegin_Immediate(const std::string& name) = 0;
		virtual void EventEnd_Immediate() = 0;
		virtual void SetMarker_Immediate(const std::string &name) = 0;
		virtual void GenerateMips_Immediate(GPUResource* ShaderResourceView) = 0;
		/////////////////////////////////////////////////////////////////////////////////

		virtual void* GetDevice() = 0;

	};

}