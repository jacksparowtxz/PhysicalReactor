#pragma once
#include "Render/GraphicsResource.h"




namespace PRE
{
/*
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	};*/
	class RenderWorld
	{
	public:
		RenderWorld(HWND windows);

		void BeginRender();
		void RenderFrame();
		void EndRender();
		void Update();
	/*	GPUBuffer* mVertexBuffer;
		GPUBuffer* mIndexBuffer;

		VertexShader* vs;
		PixelShader* ps;

		VertexLayout *pinputlayout;
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		GPUBuffer* constbuffer;
		GraphicPSO* pso;*/

	};
}