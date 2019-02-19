#pragma once
#include "Render/GraphicsResource.h"
#include "Allocator/Allocator.h"
#include "Container/vector.h"
#include "Render/Renderable.h"
#include "Render/Camera.h"
#include<windows.h>

using namespace PRE;
namespace PRE
{

	struct RenderConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 padding;
		DirectX::XMFLOAT4X4 padding;
		DirectX::XMFLOAT4X4 padding;
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
		~RenderWorld();
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
		void AddStaticMesh(StaticMesh* sm);
		Allocator* allocator;

		Vector<StaticMesh*> StaticmeshList;

		RenderConstantBuffer* m_constantBufferData[9];
		GPUBuffer* constbuffer;
		Camera *camera;
		POINT mLastMousePos;
		float dt;
		GraphicPSO* PSO;
		HANDLE Handle[9];
	};
}