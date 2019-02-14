#pragma once
#include "Render/GraphicsResource.h"
#include "Allocator/Allocator.h"
#include "Container/vector.h"
#include "Render/Renderable.h"



using namespace PRE;
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
		RenderWorld(HWND windows,Allocator* allocator);

		void BeginRender();
		void RenderFrame();
		void EndRender();
		void Update();


		Allocator* allocator;

		Vector<StaticMesh*> StaticmeshList;

		

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