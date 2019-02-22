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
		allocatorFC::deallocateDelete<Camera>(*allocator, camera);
		allocatorFC::deallocateDelete<ShaderManager>(*allocator, Renderer::shadermanager);
		for (uint32_t i = 0; i < 9; i++)
		{
			allocatorFC::deallocateDelete<RenderConstantBuffer>(*allocator, m_constantBufferData[i]);
		}
		
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
			
			GraphicPSO PSO;
			Renderer::shadermanager->GetPSO(nullptr, &PSO);
			Renderer::GetDevice()->BindGraphicsPSO(&PSO);
			UINT pFisrtConstant1 = 0;
			UINT pNumberConstant1 = 64;
			Renderer::GetDevice()->BindConstantBuffer(VS_STAGE, constbuffer, 0, &pFisrtConstant1, &pNumberConstant1);
			UINT pFisrtConstant2 = 16;
			UINT pNumberConstant2 = 16;
			Renderer::GetDevice()->BindConstantBuffer(VS_STAGE, constbuffer, 1, &pFisrtConstant2, &pNumberConstant2);
			DirectX::XMStoreFloat4x4(&m_constantBufferData[ThreadID]->model, XMMatrixTranspose(XMMatrixRotationY(90.f)));
			for (SubMesh* submesh : sm->Meshs)
			{
				UINT stride = sizeof(Vertex);
				UINT offset = 0;
				RenderMaterial(PS_STAGE, submesh);
				Renderer::GetDevice()->BindVertexBuffers(&submesh->mVertexBuffer, 0, 1, &stride, &offset);
				Renderer::GetDevice()->BindIndexBuffer(submesh->mIndexBuffer, INDEXBUFFER_32BIT, 0);
				Renderer::GetDevice()->DrawIndexed(submesh->Indices.size(), 0, 0);
			}
			Renderer::GetDevice()->FinishComanlist();
			SetEvent(Handle[ThreadID]);
		};
		RenderStaticMesh = lambda;
		JobScheduler::Wait(parallel_for(*StaticmeshList.data, StaticmeshList.Size(), RenderStaticMesh, (void*)nullptr));
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
			DirectX::XMStoreFloat4x4(&m_constantBufferData[i]->projection, XMMatrixTranspose(camera->Proj()));
			DirectX::XMStoreFloat4x4(&m_constantBufferData[i]->view, XMMatrixTranspose(camera->View()));
		}
	}

	void RenderWorld::ReSize(int width, int height)
	{
		Renderer::GetDevice()->SetResolution(width, height);
		camera->SetLens(0.45f*MathHelper::Pi, (float)(width / height), 0.01f, 1000.0f);
	}

	void RenderWorld::MoveForWard(float Direction)
	{
		camera->Walk(dt * 10 * Direction);
	}

	void RenderWorld::MoveRight(float Direction)
	{
       camera->Strafe(dt * 10 * Direction);
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

	void RenderWorld::RenderMaterial(SHADERSTAGE stage, SubMesh* submesh)
	{
		if (submesh->material->BaseColorMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->BaseColorMap, 0);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->BaseColorSampler, 0, 1);
		}
		if (submesh->material->MetalicMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->MetalicMap, 1);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->MetalicSampler, 1, 1);
		}
		if (submesh->material->SpecularMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->SpecularMap, 2);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->SpecularSampler, 2, 1);
		}
		if (submesh->material->RoughnessMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->RoughnessMap, 3);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->RoughnessSampler, 3, 1);
		}
		if (submesh->material->EmissiveMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->EmissiveMap, 4);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->EmissiveSampler, 4, 1);
		}
		if (submesh->material->OpacityMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->OpacityMap, 5);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->OpacitySampler, 5, 1);
		}
		if (submesh->material->OpcaityMaskMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->OpcaityMaskMap, 6);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->OpcaityMaskSampler, 6, 1);
		}
		if (submesh->material->NormalMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->NormalMap, 7);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->NormalSampler, 7, 1);
		}
		if (submesh->material->WorldPositionOffset != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->WorldPositionOffset, 8);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->WorldPositionOffsetSampler, 8, 1);
		}
		if (submesh->material->WorldDisplacement != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->WorldDisplacement, 9);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->WorldDisplacementSampler, 9, 1);
		}
		if (submesh->material->TessellationMultiplerMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->TessellationMultiplerMap, 10);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->TessellationMultiplerSampler, 10, 1);
		}
		if (submesh->material->SubsurfaceMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->SubsurfaceMap, 11);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->SubsurfaceSampler, 11, 1);
		}
		if (submesh->material->AmbientMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->AmbientMap, 12);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->AmbientSampler, 12, 1);
		}
		if (submesh->material->RefractionMap != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->RefractionMap, 13);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->RefractionSampler, 13, 1);
		}
		if (submesh->material->PixelDepthOffset != nullptr)
		{
			Renderer::GetDevice()->BindResource(stage, submesh->material->PixelDepthOffset, 14);
			Renderer::GetDevice()->BindSampler(stage, submesh->material->PixelDepthOffsetSampler, 14, 1);
		}
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

		camera->SetPosition(0.0f, 30.0f, -300.0f);
		camera->SetLens(0.45f*MathHelper::Pi, (float)(SCREENWIDTH/SCREENHEIGHT), 0.01f, 1000.0f);
		
		mLastMousePos.x = 0;
		mLastMousePos.y = 0;


		JobScheduler::Initialize();
		for (uint32_t i = 0; i < 9; ++i)
		{
			m_constantBufferData[i] = allocatorFC::allocateNew<RenderConstantBuffer>(*allocator);
			Handle[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
		}
	}

}