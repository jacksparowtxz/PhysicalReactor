#pragma once
#include "Render/GraphicsResource.h"
#include "Allocator/Allocator.h"
#include "Allocator/DynamicLinearAllocator.h"
#include "Container/vector.h"
#include "Render/Renderable.h"
#include "Render/Camera.h"
#include "Render/DrawKey.h"
#include "MISC/MiscHelper.h"
#include<windows.h>

using namespace PRE;
namespace PRE
{

	struct RenderConstantBuffer
	{
		RenderConstantBuffer()
		{
			
			DirectX::XMStoreFloat4x4(&view, XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&projection, XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&textransform, XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&worldinvtranspose, XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&model, XMMatrixIdentity());
		}

		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 textransform;
		DirectX::XMFLOAT4X4 worldinvtranspose;

		DirectX::XMFLOAT4X4 model;
	

		
	};

	 thread_local std::vector<StaticMesh*> LStaticMeshList;
	 thread_local std::vector<StaticMesh*> LTStaticMeshList;
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
		void RenderMaterial(SHADERSTAGE stage,SubMesh* submesh);
		void InitViews();
		Allocator* allocator;

		Vector<StaticMesh*> StaticmeshList;

		Vector<StaticMesh*> VisiblityMesh;
		Vector<StaticMesh*> TVisiblityMesh;

		RenderConstantBuffer* m_constantBufferData[9];
		GPUBuffer* constbuffer;
		Camera *camera;
		POINT mLastMousePos;
		double dt;
		HANDLE Handle[9];
		uint64_t ListPtr[9];
		uint64_t TListPtr[9];
	};
}