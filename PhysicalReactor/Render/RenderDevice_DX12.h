#pragma once
#include "CommonInclude.h"
#include "RenderDevice.h"
#include "JobSystem/JobScheduler.h"
#include "JobSystem/ParallelFor.h"
#include <codecvt>
#include "MISC/D3D12MemAlloc.h"
#include "Containers.h"
#include <dxgi1_4.h>
#include <d3d12.h>
#include <wrl/client.h>

#include "SpinLock.h"

#include<unordered_map>
#include<deque>
#include<atomic>
#include<mutex>



namespace PRE
{
	class RenderDevice_DX12 :public RenderDevice
	{
	private:


		Microsoft::WRL::ComPtr<ID3D12Device6> device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> directqueu;
		Microsoft::WRL::ComPtr<ID3D12Fence1> frameFence;

		HANDLE frameFenceEvent;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> copyQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> copyAllocator;
		Microsoft::WRL::ComPtr<ID3D12CommandList> copyCommandList;
		Microsoft::WRL::ComPtr<ID3D12Fence1> copyFence;
		HANDLE copyFenceEventl;
		uint64_t cpoyFemceValue;
		SpinLock copyQueueLock;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> graphicRootSignature;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> computeRootSignature;

		Microsoft::WRL::ComPtr<ID3D12CommandSignature> dispatchIndirectCommandSignature;
		Microsoft::WRL::ComPtr<ID3D12CommandSignature> drawInstanceIndirectCommandSignature;
		Microsoft::WRL::ComPtr<ID3D12CommandSignature> drawIndexedInstanceIndirectCommandSignature;

		Microsoft::WRL::ComPtr<ID3D12QueryHeap> querypool_timestamp;
		Microsoft::WRL::ComPtr<ID3D12QueryHeap> querypool_occlusion;
		static const size_t timestamp_query_count = 1024;
		static const size_t occlusion_query_count = 1024;
		Microsoft::WRL::ComPtr<ID3D12Resource1>	querypool_timestamp_readback;
		Microsoft::WRL::ComPtr<ID3D12Resource1> querypool_occlusion_readback;
		D3D12MA::Allocation* allocation_querypool_timestamp_readback = nullptr;
		D3D12MA::Allocation* allocation_querypool_occlusion_readback = nullptr;

		struct DescriptorAllocator
		{
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;
			size_t heap_begin;
			uint32_t itemCount;
			uint32_t maxCount;
			uint32_t itemSize;
			bool* itemAlive = nullptr;
			uint32_t lastAlloc;
			SpinLock lock;

			void init(ID3D12Device6* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t maxCount);
			~DescriptorAllocator();
			D3D12_CPU_DESCRIPTOR_HANDLE allocate();
			void clear();
			void free(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);

		};


		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> null_resource_heap_cpu;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> null_sampler_heap_cpu;
		D3D12_CPU_DESCRIPTOR_HANDLE null_resource_heap_cpu_start = {};
		D3D12_CPU_DESCRIPTOR_HANDLE null_sampler_heap_cpu_start = {};

		uint32_t resource_descrtptor_size = 0;
		uint32_t resource_descrtptor_size = 0;


		struct FrameResources
		{
			Microsoft::WRL::ComPtr<ID3D12Resource1> backBuffer;
			D3D12_CPU_DESCRIPTOR_HANDLE backBufferRTV = {};
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocators[COMMANDLIST_COUNT];
			Microsoft::WRL::ComPtr<ID3D12CommandList> commandLists[COMMANDLIST_COUNT];

			struct DescriptorTableFrameAllocator
			{
				RenderDevice_DX12* device = nullptr;
				struct DescritorHeap
				{
					D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
					Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_GPU;
					D3D12_CPU_DESCRIPTOR_HANDLE start_cpu = {};
					D3D12_CPU_DESCRIPTOR_HANDLE startgpu = {};
					uint32_t ringoffset = 0;
				};
				std::vector<DescritorHeap> heaps_resource;
				std::vector<DescritorHeap>heaps_sampler;
				size_t currentheap_resource = 0;
				size_t currentheap_sampler = 0;
				bool heaps_bound = false;

				struct Table
				{
					const GPUBuffer* CBV[GPU_RESOURCE_HEAP_CBV_COUNT];
					const GPUResource* SRV[GPU_RESOURCE_HEAP_SRV_COUNT];
					int SRV_index[GPU_RESOURCE_HEAP_SRV_COUNT];
					const GPUResource* UAV[GPU_RESOURCE_HEAP_UAV_COUNT];
					int UAV_index[GPU_RESOURCE_HEAP_UAV_COUNT];
					const Sampler* SAM[GPU_SAMPLER_HEAP_COUNT];

					bool dirty_resources;
					bool dirty_samplers;

					void reset()
					{
						::memset(CBV, 0, sizeof(CBV));
						::memset(SRV, 0, sizeof(SRV));
						::memset(SRV_index, -1, sizeof(SRV_index));
						::memset(UAV, 0, sizeof(UAV));
						::memset(UAV_index, -1, sizeof(UAV_index)); 
						::memset(SAM, 0, sizeof(SAM));
						dirty_resources = true;
						dirty_samplers = true;
					
					}

				}tables[SHADERTYPE_COUNT];

				void init(RenderDevice_DX12* device);
				void reset();
				void validate(CommandList cmd);
				void create_or_bind_heaps_on_demand(CommandList cmd);

			};
			DescriptorTableFrameAllocator descriptors[COMMANDLIST_COUNT];

			struct ResourceFrameAllocator
			{
				GPUBuffer buffer;
				uint8_t* dataBegin = nullptr;
				uint8_t* dataCur = nullptr;
				uint8_t* dataEnd = nullptr;

				void init(RenderDevice_DX12* device, size_t size);
				uint8_t* allocate(size_t dataSize, size_t alignment);
				void clear();
				uint64_t calculateOffset(uint8_t* address);
			};
			ResourceFrameAllocator resourceBuffer[COMMANDLIST_COUNT];
		};
		FrameResources frames[BACKBUFFER_COUNT];
		FrameResources& GetFrameResources()
		{
			return frames[GetFrameCount() % BACKBUFFER_COUNT];
		}

		inline ID3D12GraphicsCommandList5* GetDirecCommandList(CommandList cmd)
		{
			return static_cast<ID3D12GraphicsCommandList5*>(GetFrameResources().commandLists[cmd].Get());
		}

		struct DyanmicResourceState
		{
			GPUAllocation allocation;
			bool binding[SHADERTYPE_COUNT] = {};


		};
	public:
		RenderDevice_DX12(HWND mainscreen,bool fullscreen = false, bool debuglayer = false);
		virtual ~RenderDevice_DX12();

		
		


		bool CreateBuffer(const GPUBufferDesc *pDesc, const SubresourceData* pInitialData, GPUBuffer *pBuffer) override;
		bool  CreateTexture(const TextureDesc* pDesc, const SubresourceData* pInitialData, Texture**ppTexture) override;
		bool CreateInputLayout(const VertexLayoutDesc *pInputElementDescs, uint32_t NumElements, const Shader* shader, VertexLayout *pInputLayout) override;
		bool CreateShader(SHADERSTAGE stage, const void *pShaderBytecode, size_t BytecodeLength, Shader *pShader) override;
		bool CreateBlendState(const BlendStateDesc *pBlendStateDesc, BlendState *pBlendState) override;
		bool CreateDepthStencilState(const DepthStencilStateDesc *pDepthStencilStateDesc, DepthStencilState *pDepthStencilState) override;
		bool CreateRasterizerState(const RasterizerStateDesc *pRasterizerStateDesc, RasterizerState *pRasterizerState) override;
		bool CreateSamplerState(const SamplerDesc *pSamplerDesc, Sampler *pSamplerState) override;
		bool CreateQuery(const GPUQueryDesc *pDesc, GPUQuery *pQuery) override;
		bool CreatePipelineState(const PipelineStateDesc* desc, PipelineState pso) override;
		bool CreateRenderPass(const RenderPassDesc* pDesc, RenderPass* renderpass) override;
		
		///////////
		//////////
		
		int CreateSubresource(Texture* texture, SUBRESOURCE_TYPE type, uint32_t firstSlice, uint32_t sliceCount, uint32_t firstMip, uint32_t mipCount) override;

		bool DownloadResource(const GPUResource* resourceToDownload, const GPUResource* resourceDest, void* dataDest) override;

		void SetName(GPUResource* pResource, const char* name) override;

		void PresentBegin(CommandList cmd) override;
		void PresentEnd(CommandList cmd) override;

		virtual CommandList BeginCommandList() override;

		void WaitForGPU() override;
		virtual void ClearPipelineStateCache() override;

		void SetResolution(int width, int height) override;

		Texture GetBackBuffer() override;

		 void RenderPassBegin(const RenderPass* renderpass, CommandList cmd) = 0;
		 void RenderPassEnd(CommandList* cmd) = 0;
		 void BindScissorRect(uint32_t numRects, const Rect* rects, CommandList cmd) = 0;
		 void BindViewPorts(uint32_t NumViewPorts, const ViewPort* pViewports, CommandList cmd) = 0;
		 void BindResource(SHADERSTAGE stage, const GPUResource* resoucrces, uint32_t slot, CommandList cmd, int subresource = -1) = 0;
		 void BindResources(SHADERSTAGE stage, const GPUResource *const* resources, uint32_t slot, uint32_t count, CommandList cmd) = 0;
		 void BindUAV(SHADERSTAGE stage, const GPUResource* resource, uint32_t slot, CommandList cmd, int subresource = -1) = 0;
		 void BindUAVs(SHADERSTAGE stage, const GPUResource *const* resources, uint32_t slot, uint32_t count, CommandList cmd) = 0;
		 void UnbindResources(uint32_t slot, uint32_t num, CommandList cmd) = 0;
		 void UnbindUAVs(uint32_t slot, uint32_t num, CommandList cmd) = 0;
		 void BindSampler(SHADERSTAGE stage, const Sampler* sampler, uint32_t slot, CommandList cmd) = 0;
		 void BindConstantBuffer(SHADERSTAGE stage, const GPUBuffer* buffer, uint32_t slot, CommandList cmd) = 0;
		 void BindVertexBuffers(const GPUBuffer *const* vertexBuffers, uint32_t slot, uint32_t count, const uint32_t* strides, const uint32_t* offsets, CommandList cmd) = 0;
		 void BindIndexBuffer(const GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, uint32_t offset, CommandList cmd) = 0;
		 void BindStencilRef(uint32_t value, CommandList cmd) = 0;
		 void BindBlendFactor(float r, float g, float b, float a, CommandList cmd) = 0;
		 void BindPipelineState(const PipelineState* pso, CommandList cmd) = 0;
		 void BindComputeShader(const Shader* cs, CommandList cmd) = 0;
		 void Draw(uint32_t vertexCount, uint32_t startVertexLocation, CommandList cmd) = 0;
		 void DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, CommandList cmd) = 0;
		 void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation, CommandList cmd) = 0;
		 void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, uint32_t startInstanceLocation, CommandList cmd) = 0;
		 void DrawInstancedIndirect(const GPUBuffer* args, uint32_t args_offset, CommandList cmd) = 0;
		 void DrawIndexedInstancedIndirect(const GPUBuffer* args, uint32_t args_offset, CommandList cmd) = 0;
		 void Dispatch(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ, CommandList cmd) = 0;
		 void DispatchIndirect(const GPUBuffer* args, uint32_t args_offset, CommandList cmd) = 0;
		 void CopyResource(const GPUResource* pDst, const GPUResource* pSrc, CommandList cmd) = 0;
		 void CopyTexture2D_Region(const Texture* pDst, uint32_t dstMip, uint32_t dstX, uint32_t dstY, const Texture* pSrc, uint32_t srcMip, CommandList cmd) = 0;
		 void MSAAResolve(const Texture* pDst, const Texture* pSrc, CommandList cmd) = 0;
		 void UpdateBuffer(const GPUBuffer* buffer, const void* data, CommandList cmd, int dataSize = -1) = 0;
		 void QueryBegin(const GPUQuery *query, CommandList cmd) = 0;
		 void QueryEnd(const GPUQuery *query, CommandList cmd) = 0;
		 bool QueryRead(const GPUQuery *query, GPUQueryResult* result) = 0;
		 void Barrier(const GPUBarrier* barriers, uint32_t numBarriers, CommandList cmd) = 0;

		///////////
		///////
		
		 void EventBegin(const char* name, CommandList cmd) override;
		 void EventEnd(CommandList cmd) override;
		 void SetMarker(const char* name, CommandList cmd) override;

		

		 GPUAllocation AllocateGPU(size_t dataSize, CommandList cmd) override;

		 struct AllocationHandler
		 {
			 D3D12MA::Allocation* allocator = nullptr;
			 Microsoft::WRL::ComPtr<ID3D12Device6> device;
			 uint64_t framecount = 0;
			 std::mutex destroylocker;
			 std::deque<std::pair<D3D12MA::Allocation*, uint64_t>> destoryer_allocations;
			 std::deque<std::pair<Microsoft::WRL::ComPtr<ID3D12Resource>, uint64_t>> destroyer_resources;
			 std::deque<std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, uint64_t>> destroyer_resourceviews;
			 std::deque<std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, uint64_t>> destroyer_rtvs;
			 std::deque<std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, uint64_t>> destroyer_dsvs;
			 std::deque<std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, uint64_t>> destroyer_samplers;
			 std::deque<std::pair<uint32_t, uint64_t>> destroyer_queries_timestamp;
			 std::deque<std::pair<uint32_t, uint64_t>> destroyer_queries_occlusion;
			 std::deque<std::pair<Microsoft::WRL::ComPtr<ID3D12PipelineState>, uint64_t>> destroyer_pipelines;

			 DescriptorAllocator RTAllocator;
			 DescriptorAllocator DSAllocator;
			 DescriptorAllocator ResourceAllocator;
			 DescriptorAllocator SamplerAllocator;

			 ThreadSafeRingBuffer<uint32_t, timestamp_query_count> free_timestampqueires;
			 ThreadSafeRingBuffer<uint32_t, occlusion_query_count> free_occlusionqueires;


			 ~AllocationHandler()
			 {
				 Update(~0, 0);
				 if (allocator)
					 allocator->Release();
			 }

			 void Update(uint64_t FREAMECOUNT, uint32_t BACKBUFFER_COUNT)
			 {
				 destroylocker.lock();
				 while (!destoryer_allocations.empty())
				 {
					 if (destoryer_allocations.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 auto item = destoryer_allocations.front();
						 destoryer_allocations.pop_front();
						 item.first->Release();
					 }
					 else
					 {
						 break;
					 }
				 }
				 while (!destroyer_resources.empty())
				 {
					 if (destroyer_resources.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 destroyer_resources.pop_front();
					 }
					 else
					 {
						 break;
					 }
				 }
				 while (!destroyer_resourceviews.empty())
				 {
					 if (destroyer_resourceviews.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 auto item = destroyer_resourceviews.front();
						 destroyer_resourceviews.pop_front();
						 ResourceAllocator.free(item.first);
					 }
					 else
					 {
						 break;
					 }
				 }
				 while (!destroyer_rtvs.empty())
				 {
					 if (destroyer_rtvs.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 auto item = destroyer_rtvs.front();
						 destroyer_rtvs.pop_front();
						 RTAllocator.free(item.first);
					 }
					 else
					 {
						 break;
					 }
				 }
				 while (!destroyer_dsvs.empty())
				 {
					 if (destroyer_dsvs.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 auto item = destroyer_dsvs.front();
						 destroyer_dsvs.pop_front();
						 DSAllocator.free(item.first);
					 }
					 else
					 {
						 break;
					 }
				 }
				 while (!destroyer_samplers.empty())
				 {
					 if (destroyer_samplers.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 auto item = destroyer_samplers.front();
						 destroyer_samplers.pop_front();
						 SamplerAllocator.free(item.first);
					 }
					 else
					 {
						 break;
					 }
				 }
				 while (!destroyer_queries_occlusion.empty())
				 {
					 if (destroyer_queries_occlusion.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 auto item = destroyer_queries_occlusion.front();
						 destroyer_queries_occlusion.pop_front();
						 free_occlusionqueires.push_back(item.first);
					 }
					 else
					 {
						 break;
					 }
				 }
				 while (!destroyer_queries_timestamp.empty())
				 {
					 if (destroyer_queries_timestamp.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 auto item = destroyer_queries_timestamp.front();
						 destroyer_queries_timestamp.pop_front();
						 free_timestampqueires.push_back(item.first);
					 }
					 else
					 {
						 break;
					 }
				 }
				 while (!destroyer_pipelines.empty())
				 {
					 if (destroyer_pipelines.front().second + BACKBUFFER_COUNT < FREAMECOUNT)
					 {
						 destroyer_pipelines.pop_front();
						 // comptr auto delete
					 }
					 else
					 {
						 break;
					 }
				 }
				 destroylocker.unlock();
			 }
		 };
		
		 std::shared_ptr <AllocationHandler> allocationhanlder;
	};
}