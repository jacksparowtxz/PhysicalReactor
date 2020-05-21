#pragma once
#include "Render/RenderDevice.h"
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"

#include"vulkan/vulkan_win32.h"

#include<optional>
#include<mutex>




namespace PRE
{

	struct QueueFamilyIndices 
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
	    
		bool isComplete()
		{
			return graphicsFamily.has_value()&&presentFamily.has_value();
		}
	};


	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	
	
	};


	class RenderDevice_Vulkan :public RenderDevice
	{
	public:

		RenderDevice_Vulkan(HWND mainscreen, bool fullscreen = false, bool debuglayer = false);
		~RenderDevice_Vulkan();


	private:

		VkInstance instance;
		VkDebugReportCallbackEXT callback;
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		QueueFamilyIndices queueIndices;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkPhysicalDeviceProperties physicalDeviceProperties;

		VkQueue copyQueue;
		VkCommandPool copyCommandPool;
		VkCommandBuffer copyCommandBuffer;
		VkFence copyFence;
		std::mutex copyQueueLock;

		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;

		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;

		VkRenderPass defaultRenderpass;
		VkPipelineLayout defaultPiplineLayout_Graphics;
		VkPipelineLayout defaultPipelineLayout_Compute;
		VkDescriptorSetLayout defaultDescriptorSetLayouts[STAGE_COUNT];
		uint32_t descriptorCount;

		bool CreateBuffer(const GPUBufferDesc *pDesc, const SubresourceData* pInitialData, GPUBuffer *pBuffer) override;
		bool CreateTexture(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture *pTexture) override;
		bool CreateInputLayout(const InputLayoutDesc *pInputElementDescs, uint32_t NumElements, const Shader* shader, InputLayout *pInputLayout) override;
		bool CreateShader(SHADERSTAGE stafe, const void *pShaderBytecode, size_t BytecodeLength, Shader *pShader) override;
		bool CreateBlendState(const BlendStateDesc *pBlendStateDesc, BlendState *pBlendState) override;
		bool CreateDepthStencilState(const DepthStencilStateDesc *pDepthStencilStateDesc, DepthStencilState *pDepthStencilState) override;
		bool CreateRasterizerState(const RasterizerStateDesc *pRasterizerStateDesc, RasterizerState *pRasterizerState) override;
		bool CreateSampler(const SamplerDesc *pSamplerDesc, Sampler *pSamplerState) override;
		bool CreateQuery(const GPUQueryDesc *pDesc, GPUQuery *pQuery) override;
		bool CreatePipelineState(const PipelineStateDesc* pDesc, PipelineState* pso) override;
		bool CreateRenderPass(const RenderPassDesc* pDesc, RenderPass* renderpass) override;

		int CreateSubresource(Texture* texture, SUBRESOURCE_TYPE type, uint32_t firstSlice, uint32_t sliceCount, uint32_t firstMip, uint32_t mipCount) override;

		bool DownloadResource(const GPUResource* resourceToDownload, const GPUResource* resourceDest, void* dataDest) override;

		void SetName(GPUResource* pResource, const char* name) override;

		void PresentBegin(CommandList cmd) override;
		void PresentEnd(CommandList cmd) override;

		virtual CommandList BeginCommandList() override;

		void WaitForGPU() override;
		void ClearPipelineStateCache() override;

		void SetResolution(int width, int height) override;

		Texture GetBackBuffer() override;

		///////////////Thread-sensitive////////////////////////

		void RenderPassBegin(const RenderPass* renderpass, CommandList cmd) override;
		void RenderPassEnd(CommandList cmd) override;
		void BindScissorRects(uint32_t numRects, const Rect* rects, CommandList cmd) override;
		void BindViewports(uint32_t NumViewports, const Viewport *pViewports, CommandList cmd) override;
		void BindResource(SHADERSTAGE stage, const GPUResource* resource, uint32_t slot, CommandList cmd, int subresource = -1) override;
		void BindResources(SHADERSTAGE stage, const GPUResource *const* resources, uint32_t slot, uint32_t count, CommandList cmd) override;
		void BindUAV(SHADERSTAGE stage, const GPUResource* resource, uint32_t slot, CommandList cmd, int subresource = -1) override;
		void BindUAVs(SHADERSTAGE stage, const GPUResource *const* resources, uint32_t slot, uint32_t count, CommandList cmd) override;
		void UnbindResources(uint32_t slot, uint32_t num, CommandList cmd) override;
		void UnbindUAVs(uint32_t slot, uint32_t num, CommandList cmd) override;
		void BindSampler(SHADERSTAGE stage, const Sampler* sampler, uint32_t slot, CommandList cmd) override;
		void BindConstantBuffer(SHADERSTAGE stage, const GPUBuffer* buffer, uint32_t slot, CommandList cmd) override;
		void BindVertexBuffers(const GPUBuffer *const* vertexBuffers, uint32_t slot, uint32_t count, const uint32_t* strides, const uint32_t* offsets, CommandList cmd) override;
		void BindIndexBuffer(const GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, uint32_t offset, CommandList cmd) override;
		void BindStencilRef(uint32_t value, CommandList cmd) override;
		void BindBlendFactor(float r, float g, float b, float a, CommandList cmd) override;
		void BindPipelineState(const PipelineState* pso, CommandList cmd) override;
		void BindComputeShader(const Shader* cs, CommandList cmd) override;
		void Draw(uint32_t vertexCount, uint32_t startVertexLocation, CommandList cmd) override;
		void DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, CommandList cmd) override;
		void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation, CommandList cmd) override;
		void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, uint32_t startInstanceLocation, CommandList cmd) override;
		void DrawInstancedIndirect(const GPUBuffer* args, uint32_t args_offset, CommandList cmd) override;
		void DrawIndexedInstancedIndirect(const GPUBuffer* args, uint32_t args_offset, CommandList cmd) override;
		void Dispatch(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ, CommandList cmd) override;
		void DispatchIndirect(const GPUBuffer* args, uint32_t args_offset, CommandList cmd) override;
		void CopyResource(const GPUResource* pDst, const GPUResource* pSrc, CommandList cmd) override;
		void CopyTexture2D_Region(const Texture* pDst, uint32_t dstMip, uint32_t dstX, uint32_t dstY, const Texture* pSrc, uint32_t srcMip, CommandList cmd) override;
		void MSAAResolve(const Texture* pDst, const Texture* pSrc, CommandList cmd) override;
		void UpdateBuffer(const GPUBuffer* buffer, const void* data, CommandList cmd, int dataSize = -1) override;
		void QueryBegin(const GPUQuery *query, CommandList cmd) override;
		void QueryEnd(const GPUQuery *query, CommandList cmd) override;
		bool QueryRead(const GPUQuery* query, GPUQueryResult* result) override;
		void Barrier(const GPUBarrier* barriers, uint32_t numBarriers, CommandList cmd) override;

		void EventBegin(const char* name, CommandList cmd) override;
		void EventEnd(CommandList cmd) override;
		void SetMarker(const char* name, CommandList cmd) override;
	};
}


