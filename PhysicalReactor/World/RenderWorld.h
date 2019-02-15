#pragma once
#include "Render/GraphicsResource.h"
#include "Allocator/Allocator.h"
#include "Container/vector.h"
#include "Render/Renderable.h"
#include "Render/Camera.h"


using namespace PRE;
namespace PRE
{

	struct RenderConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};
	/*
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
		void Update(float deltatime);
		void RenderScene();
		void Initilize(HWND windows, Allocator* allocator);
		void ReSize(int width, int height);
		void MoveForWard(float Direction);
		void MoveRight(float Direction);
		void CameraRotation(int x, int y);
		Allocator* allocator;

		Vector<StaticMesh*> StaticmeshList;

		RenderConstantBuffer m_constantBufferData;
		GPUBuffer* constbuffer;
		Camera *camera;
		POINT mLastMousePos;
		float dt;
	/*	GPUBuffer* mVertexBuffer;
		GPUBuffer* mIndexBuffer;

		VertexShader* vs;
		PixelShader* ps;

		VertexLayout *pinputlayout;
		
		
		GraphicPSO* pso;*/

	};
}