#include "World/RenderWorld.h"
#include "Render/Renderer.h"
#include "Render/RenderDevice_DX11.h"
#include "MISC/Mathhelper.h"
#include "JobSystem/ParallelFor.h"
#include "JobSystem/JobScheduler.h"

#include <functional>

using namespace std;

namespace PRE
{
	RenderWorld::RenderWorld(HWND windows, Allocator* Inallocator, DynamicLinearAllocator* allocator1) :allocator(Inallocator),StaticmeshList(*allocator1)
	{
		Initilize(windows,allocator);
		
	}


	RenderWorld::~RenderWorld()
	{
		JobScheduler::Shutdown();
		allocatorFC::deallocateDelete<GraphicPSO>(*allocator, PSO);
		allocatorFC::deallocateDelete<Camera>(*allocator, camera);
		allocatorFC::deallocateDelete<ShaderManager>(*allocator, Renderer::shadermanager);
		allocatorFC::deallocateArray<RenderConstantBuffer>(*allocator,*m_constantBufferData);
	}

	void RenderWorld::BeginRender()
	{
		Renderer::GetDevice()->PresentBegin();
		Renderer::GetDevice()->SetResolution(1920,1080);
	}


	void RenderWorld::RenderFrame()
	{
		function<void(StaticMesh*, uint32_t, void*)> RenderStaticMesh;
		auto lambda = [&, this](StaticMesh* sm, uint32_t size, void* ExtraData) {

			Renderer::GetDevice()->UpdateBuffer(constbuffer, m_constantBufferData[ThreadID]);
			UINT pFisrtConstant1 = 0;
			UINT pNumberConstant1 = 16;
			
			GraphicPSO* pso = (GraphicPSO*)ExtraData;
			Renderer::GetDevice()->BindGraphicsPSO(pso);
			Renderer::GetDevice()->BindConstantBuffer(VS_STAGE, constbuffer, 0, nullptr, nullptr);
			/*UINT pFisrtConstant2 = 256;
			UINT pNumberConstant2 = 32;
			Renderer::GetDevice()->BindConstantBuffer(VS_STAGE, constbuffer, 0, &pFisrtConstant2, &pNumberConstant2);*/
			DirectX::XMStoreFloat4x4(&m_constantBufferData[ThreadID]->model, XMMatrixTranspose(XMMatrixRotationY(90.f)));
			for (SubMesh* submesh : sm->Meshs)
			{
				UINT stride = sizeof(Vertex);
				UINT offset = 0;
				Renderer::GetDevice()->BindVertexBuffers(&submesh->mVertexBuffer, 0, 1, &stride, &offset);
				Renderer::GetDevice()->BindIndexBuffer(submesh->mIndexBuffer, INDEXBUFFER_16BIT, 0);
				Renderer::GetDevice()->DrawIndexed(submesh->Indices.size(), 0, 0);
			}
			Renderer::GetDevice()->FinishComanlist();
			SetEvent(Handle[ThreadID]);
		};
		RenderStaticMesh = lambda;
		Renderer::shadermanager->GetPSO(nullptr, PSO);
		JobScheduler::Wait(parallel_for(*StaticmeshList.data, StaticmeshList.Size(), RenderStaticMesh,(void*)PSO));
	}

	void RenderWorld::EndRender()
	{
		WaitForMultipleObjects(9L,Handle,TRUE,INFINITY);
		Renderer::GetDevice()->ExcuteDeferredContexts();
		Renderer::GetDevice()->PresentEnd();
	}

	void RenderWorld::Update(double deltatime)
	{
		dt = deltatime;
		camera->UpdateViewMatrix();
		for (uint32_t i=0;i<9;++i)
		{
			DirectX::XMStoreFloat4x4(&m_constantBufferData[i]->projection, camera->Proj());
			DirectX::XMStoreFloat4x4(&m_constantBufferData[i]->view, camera->View());
		}
	}

	void RenderWorld::ReSize(int width, int height)
	{
		Renderer::GetDevice()->SetResolution(width, height);
		camera->SetLens(0.45f*MathHelper::Pi, (float)(width / height), 0.1f, 5000.0f);
	}

	void RenderWorld::MoveForWard(float Direction)
	{
		camera->Walk(dt * 1000 * Direction);
	}

	void RenderWorld::MoveRight(float Direction)
	{
       camera->Strafe(dt * 1000 * Direction);
	}

	void RenderWorld::CameraRotation(WPARAM btnState,int x, int y)
	{
		if ((btnState&MK_LBUTTON) != 0)
		{
			float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
			float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

			camera->Pitch(dy);
			camera->RotateY(dx);
		}
		mLastMousePos.x = x;
		mLastMousePos.y = y;
	}

	void RenderWorld::SetMousePosition(HWND windows,int x, int y)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(windows);
	}

	void RenderWorld::AddStaticMesh(StaticMesh* sm)
	{
		StaticmeshList.Push_Back(sm);
	}

	void RenderWorld::RenderScene()
	{
		BeginRender();
		RenderFrame();
		EndRender();
	}

	void RenderWorld::Initilize(HWND windows, Allocator* allocator)
	{
		Renderer::renderdevice = new RenderDevice_DX11(windows, false, true);
		Renderer::shadermanager = allocatorFC::allocateNew<ShaderManager>(*allocator);
		Renderer::shadermanager->CreateShader();
		camera = allocatorFC::allocateNew<Camera>(*allocator);
		GPUBufferDesc constantdesc;
		constantdesc.BindFlags = BIND_CONSTANT_BUFFER;
		constantdesc.ByteWidth = sizeof(RenderConstantBuffer);
		constantdesc.CPUAccessFlags = 0;
		constantdesc.MiscFlags = 0;
		constantdesc.Usage = USAGE_DEFAULT;
		constantdesc.StructureByteStride = 0;
		constbuffer = new GPUBuffer;
		Renderer::GetDevice()->CreateBuffer(&constantdesc, nullptr, constbuffer);


		RECT rect = RECT();
		GetClientRect(windows, &rect);

		int SCREENWIDTH = rect.right - rect.left;
		int SCREENHEIGHT = rect.bottom - rect.top;

		camera->SetPosition(4000.0f, 600.0f, -50.0f);
		camera->SetLens(0.45f*MathHelper::Pi, (float)(SCREENWIDTH/SCREENHEIGHT), 0.1f, 5000.0f);
		
		mLastMousePos.x = 0;
		mLastMousePos.y = 0;

		PSO = allocatorFC::allocateNew<GraphicPSO>(*allocator);

		JobScheduler::Initialize();
		for (uint32_t i = 0; i < 9; ++i)
		{
			m_constantBufferData[i] = allocatorFC::allocateNew<RenderConstantBuffer>(*allocator);
			Handle[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
		}
	}

}