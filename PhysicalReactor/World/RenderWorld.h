#pragma once
#include "Render/GraphicsResource.h"
#include "Allocator/Allocator.h"
#include "Allocator/DynamicLinearAllocator.h"
#include "Container/vector.h"
#include "Render/Renderable.h"
#include "Render/Camera.h"
#include<windows.h>

using namespace PRE;
namespace PRE
{

	struct RenderConstantBuffer
	{
		RenderConstantBuffer()
		{
			DirectX::XMStoreFloat4x4(&model, XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&view, XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&projection, XMMatrixIdentity());
		}

		DirectX::XMFLOAT4X4 model;
		//DirectX::XMFLOAT4X4 padding1;
		//DirectX::XMFLOAT4X4 padding2;
		//DirectX::XMFLOAT4X4 padding3;
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
		RenderWorld(HWND windows,Allocator* Inallocator, DynamicLinearAllocator* allocator1);
		~RenderWorld();
		void BeginRender();
		void RenderFrame();
		void EndRender();
		void Update(double deltatime);
		void RenderScene();
		void Initilize(HWND windows, Allocator* allocator);
		void ReSize(int width, int height);
		void MoveForWard(float Direction);
		void MoveRight(float Direction);
		void CameraRotation(WPARAM btnState,int x, int y);
		void SetMousePosition(HWND windows,int x, int y);
		void AddStaticMesh(StaticMesh* sm);
		Allocator* allocator;

		Vector<StaticMesh*> StaticmeshList;

		RenderConstantBuffer* m_constantBufferData[9];
		GPUBuffer* constbuffer;
		Camera *camera;
		POINT mLastMousePos;
		double dt;
		GraphicPSO* PSO;
		HANDLE Handle[9];
	};
}