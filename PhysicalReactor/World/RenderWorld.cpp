#include "World/RenderWorld.h"
#include "Render/Renderer.h"
#include "Render/RenderDevice_DX11.h"
#include "MISC/Mathhelper.h"
#include "JobSystem/ParallelFor.h"
#include "JobSystem/JobScheduler.h"

#include <functional>

using namespace std;
thread_local std::vector<StaticMesh*> LStaticMeshList;
thread_local std::vector<StaticMesh*> LTStaticMeshList;


namespace PRE
{
	RenderWorld::RenderWorld(HWND windows)
	{
		Initilize(windows);
	}


	RenderWorld::~RenderWorld()
	{
		JobScheduler::Shutdown();
		
		SAFE_DELETE(camera);
		SAFE_DELETE(Wireframestate);
		SAFE_DELETE(Solidstate);
		SAFE_DELETE(constbuffer);
		SAFE_DELETE(SpLutSampler);
		SAFE_DELETE(tonemappingsampler);
		SAFE_DELETE(rendertarget);
		ShaderManager* tempSM = Renderer::shadermanager;
		SAFE_DELETE(tempSM);
		Renderer::shadermanager = nullptr;
		delete[] m_constantBufferData;
		delete Renderer::renderdevice;

	}

	void RenderWorld::BeginRender()
	{
		rendertarget->Clear_Immediate(false, 0);
		rendertarget->Set(false, 0);  
		
	/*	Renderer::GetDevice()->PresentBegin();
		Renderer::GetDevice()->SetResolution(1904, 1021);
		Renderer::GetDevice()->BindBackBufferRenderTargets(nullptr);*/
	
		
		RenderWireframe(false);
	}


	void RenderWorld::RenderFrame()
	{
		function<void(StaticMesh*, uint32_t, void*)> RenderStaticMesh;
		auto lambda = [&, this](StaticMesh* sm, uint32_t size, void* ExtraData) {

		Renderer::GetDevice()->UpdateBuffer(constbuffer, m_constantBufferData[ThreadID]);

			GraphicPSO PSO;
			Renderer::shadermanager->GetPSO(TYPE_STATICMESH, &PSO);
			PSO.desc.rs = rasterizerstate;
			//PSO.desc.dss = defalutDSS;
			Renderer::GetDevice()->BindGraphicsPSO(&PSO);
			UINT pFisrtConstant1 = 0;
			UINT pNumberConstant1 = 64;
			Renderer::GetDevice()->BindConstantBuffer(VS_STAGE, constbuffer, 0, &pFisrtConstant1, &pNumberConstant1);
			UINT pFisrtConstant2 = 16;
			UINT pNumberConstant2 = 16;
			Renderer::GetDevice()->BindConstantBuffer(VS_STAGE, constbuffer, 1, &pFisrtConstant2, &pNumberConstant2);
			UINT pFisrtConstant3 = 48;
			UINT pNumberConstant3 = 64;
			Renderer::GetDevice()->BindConstantBuffer(PS_STAGE, constbuffer, 0, &pFisrtConstant3, &pNumberConstant3);
			UINT pFisrtConstant4 = 64;
			UINT pNumberConstant4 = 64;
			Renderer::GetDevice()->BindConstantBuffer(PS_STAGE, constbuffer, 1, &pFisrtConstant4, &pNumberConstant4);
			UINT pFisrtConstant5 = 80;
			UINT pNumberConstant5 = 64;
			Renderer::GetDevice()->BindConstantBuffer(PS_STAGE, constbuffer, 2, &pFisrtConstant5, &pNumberConstant5);
			UINT pFisrtConstant6 = 96;
			UINT pNumberConstant6 = 16;
			Renderer::GetDevice()->BindConstantBuffer(PS_STAGE, constbuffer, 3, &pFisrtConstant6, &pNumberConstant6);
			Renderer::GetDevice()->BindSampler(PS_STAGE, SpLutSampler, 15, 1);
			Renderer::GetDevice()->BindResource(PS_STAGE, sky->EnvMap, 15);
			Renderer::GetDevice()->BindResource(PS_STAGE, sky->SpLutMap, 16);
			Renderer::GetDevice()->BindResource(PS_STAGE, sky->IradMap, 17);
			DirectX::XMStoreFloat4x4(&m_constantBufferData[ThreadID]->model, XMMatrixTranspose(XMMatrixRotationX(0.0)));
			DirectX::XMStoreFloat4x4(&m_constantBufferData[ThreadID]->worldinvtranspose, MathHelper::InverseTranspose(std::move(XMMatrixTranspose(XMMatrixRotationX(0.0)))));
			
			for (SubMesh* submesh : sm->Meshs)
			{
				UINT stride = sizeof(Vertex);
				UINT offset = 0;
				RenderMaterial(PS_STAGE, submesh);
				m_constantBufferData[ThreadID]->BaseColorFactor = submesh->material->BaseColor;
				m_constantBufferData[ThreadID]->metalic_factor = submesh->material->Metalness;
				m_constantBufferData[ThreadID]->Roughness_factor = submesh->material->Roughness;
				m_constantBufferData[ThreadID]->emissive_factor = submesh->material->Emssive;
				Renderer::GetDevice()->BindVertexBuffers(&submesh->mVertexBuffer, 0, 1, &stride, &offset);
				Renderer::GetDevice()->BindIndexBuffer(submesh->mIndexBuffer, INDEXBUFFER_32BIT, 0);
				Renderer::GetDevice()->DrawIndexed(submesh->Indices.size(), 0, 0);
			}
			//Renderer::GetDevice()->FinishComanlist();
			//SetEvent(Handle[ThreadID]);
		};
		RenderStaticMesh = lambda;
		//JobScheduler::Wait(parallel_for(*StaticmeshList.data(), StaticmeshList.Size(), RenderStaticMesh, (void*)nullptr));
		RenderStaticMesh(*StaticMeshList.data(), StaticMeshList.size(), (void*)nullptr);

		///////////////////RenderSky/////////////////////////////
		std::function<void(Sky*, uint32_t, void*)> RenderSkyFC;
		auto RenderSkybox = [&, this](Sky*sky, uint32_t size, void* extradata) {

			Renderer::GetDevice()->UpdateBuffer(constbuffer, m_constantBufferData[ThreadID]);

			GraphicPSO PSO;
			Renderer::shadermanager->GetPSO(TYPE_SKY, &PSO);
			PSO.desc.rs = sky->Skymaterial->rasterzerstate;
			PSO.desc.dss = sky->Skymaterial->depthstencilstate;
			Renderer::GetDevice()->BindGraphicsPSO(&PSO);
			UINT pFisrtConstant = 32;
			UINT pNumberConstant = 16;
			Renderer::GetDevice()->BindConstantBuffer(VS_STAGE, constbuffer, 0, &pFisrtConstant, &pNumberConstant);
			XMFLOAT3 eyePos = camera->GetPosition();
			XMMATRIX T = XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);
			XMMATRIX WVP = XMMatrixMultiply(T, camera->ViewProj());
			DirectX::XMStoreFloat4x4(&m_constantBufferData[ThreadID]->WorldViewProj, XMMatrixTranspose(WVP));
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			RenderMaterial(PS_STAGE, sky->SkyMesh->Meshs[0]);
			Renderer::GetDevice()->BindVertexBuffers(&sky->SkyMesh->Meshs[0]->mVertexBuffer, 0, 1, &stride, &offset);
			Renderer::GetDevice()->BindIndexBuffer(sky->SkyMesh->Meshs[0]->mIndexBuffer, INDEXBUFFER_32BIT, 0);
			Renderer::GetDevice()->DrawIndexed(sky->SkyMesh->Meshs[0]->Indices.size(), 0, 0);
			/*PSO.desc.rs = 0;
			PSO.desc.dss = 0;
			PSO.desc.vs = 0;
			PSO.desc.ps = 0;
			Renderer::GetDevice()->BindGraphicsPSO(&PSO);*/
			//Renderer::GetDevice()->FinishComanlist();
		};

		RenderSkyFC = RenderSkybox;
		RenderSkyFC(sky, 1, nullptr);
		
		//JobScheduler::Wait(parallel_for(sky, 1, RenderSkyFC, nullptr));

		auto tonemapping = [&, this]() {
			rendertarget->Deactivate();
			Renderer::GetDevice()->SetResolution(1904, 1021);
			Texture2D *sdt = rendertarget->depthstencil->GetTextureResolvedMSAA();
			Renderer::GetDevice()->BindBackBufferRenderTargets(sdt);
			GraphicPSO PSO;
			Renderer::shadermanager->GetPSO(OBJECTTYPE::TYPE_TONEMAPPING,&PSO);
			Renderer::GetDevice()->BindGraphicsPSO(&PSO);
			Renderer::GetDevice()->BindResource(PS_STAGE, rendertarget->GetTextureResolvedMSAA(), 0);
			Renderer::GetDevice()->BindSampler(PS_STAGE,tonemappingsampler,0,1);
			Renderer::GetDevice()->Draw(3,0);
			Renderer::GetDevice()->FinishComanlist();
		};

		tonemapping();
	}

	void RenderWorld::EndRender()
	{
		//WaitForMultipleObjects(9L,Handle,TRUE,INFINITY);
		Renderer::GetDevice()->ExcuteDeferredContexts();
		Renderer::GetDevice()->PresentEnd();
	}

	void RenderWorld::Update(double deltatime)
	{
		dt = deltatime;
		camera->UpdateViewMatrix();
		for (uint32_t i = 0; i < 9; ++i)
		{
			DirectX::XMStoreFloat4x4(&m_constantBufferData[i]->projection, XMMatrixTranspose(camera->Proj()));
			DirectX::XMStoreFloat4x4(&m_constantBufferData[i]->view, XMMatrixTranspose(camera->View()));
			DirectX::XMStoreFloat4(&m_constantBufferData[i]->EyePos, camera->GetPositionXM());
		}
	}

	void RenderWorld::ReSize(int width, int height)
	{
		Renderer::GetDevice()->SetResolution(width, height);
		camera->SetLens(0.25f*MathHelper::Pi, (float)(width / height), 0.01f, 1000.0f);
	}

	void RenderWorld::MoveForWard(float Direction)
	{
		camera->Walk(dt * 100 * Direction);
	}

	void RenderWorld::MoveRight(float Direction)
	{
		camera->Strafe(dt * 100 * Direction);
	}

	void RenderWorld::CameraRotation(WPARAM btnState, int x, int y)
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

	void RenderWorld::SetMousePosition(HWND windows, int x, int y)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(windows);
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

	void RenderWorld::InitViews(Level* currentlevel)
	{
		std::function<void(StaticMesh*, uint32_t, void*)> FrustumCull;
		auto lambda = [&, this](StaticMesh* sm, uint32_t size, void* ExtraData) {

			for (uint32_t i = 0; i < size; i++)
			{
				if (camera->GetFrustum().CheckBox(*sm->aabb) != 0)
				{
					BitSizeStruct<21> Depth;
					Depth.data = std::move(camera->GetDistance(sm->GetTransInformation(1)));
					if (sm->GetMaterialBlendMode())
					{
						DrawKey drawkey = DrawKey::GenerateKey(0, ViewLayerType::e3D, 1, sm->GetMaterialID().data, Depth.data, TranslucencyType::eOpaque, false);
						*sm->drawkey = std::move(drawkey);
						LStaticMeshList.push_back(&sm[i]);
					}
					else
					{
						DrawKey drawkey = DrawKey::GenerateKey(0, ViewLayerType::e3D, 1, sm->GetMaterialID().data, Depth.data, TranslucencyType::eNormal, false);
						*sm->drawkey = std::move(drawkey);
						LTStaticMeshList.push_back(&sm[i]);
					}
				}
			}

			ListPtr[ThreadID] = (uint64_t)&LStaticMeshList;
			TListPtr[ThreadID] = (uint64_t)&LTStaticMeshList;
		};

		FrustumCull = lambda;
		JobScheduler::Wait(parallel_for(*currentlevel->StaticMeshList.data(), currentlevel->StaticMeshList.size(), FrustumCull, nullptr, DataSizeSplitter(32 * 1024)));

		for (uint32_t i = 0; i < 9; i++)
		{
			std::vector<StaticMesh*> *slist = (std::vector<StaticMesh*>*)ListPtr[i];
			std::vector<StaticMesh*> *Tslist = (std::vector<StaticMesh*>*)TListPtr[i];


			for (uint32_t k = 0; k < slist->size(); k++)
			{
				VisiblityMesh.emplace_back(*slist->operator[](k));
			}
			for (uint32_t k = 0; k < Tslist->size(); k++)
			{
				TVisiblityMesh.emplace_back(*Tslist->operator[](k));
			}
		}

		std::function<void(StaticMesh*, uint32_t, void*)> RadixSortFC;
		auto radixsort = [&, this](StaticMesh* unorderarray, uint32_t size, void* extradata) {

			StaticMesh *radixArrays[10];
			int Ra[10];
			for (int i = 0; i < 10; i++)
			{
				radixArrays[i] = (StaticMesh *)malloc(sizeof(StaticMesh)*(size + 1));
				Ra[i] = 0;
			}
			for (int pos = 1; pos <= 10; pos++)
			{
				for (int i = 0; i < size; i++)
				{
					int num = GetDigitInPos(unorderarray[i].GetMaterialID().data, pos);
					int index = ++Ra[num];
					radixArrays[num][index] = unorderarray[i];
				}
				for (int i = 0, j = 0; i < 10; i++)
				{
					for (int k = 1; k <= Ra[i]; k++)
					{
						unorderarray[j++] = radixArrays[i][k];
					}
					radixArrays[i][0] = StaticMesh();
					Ra[i] = 0;
				}
			}
			for (int i = 0; i < 10; i++)
			{
				free(radixArrays[i]);
			}
		};

		RadixSortFC = radixsort;
		RadixSortFC(VisiblityMesh.data(), VisiblityMesh.size(), nullptr);
		RadixSortFC(TVisiblityMesh.data(), TVisiblityMesh.size(), nullptr);
	}

	void RenderWorld::RenderWireframe(bool Wireframe)
	{
		if (Wireframe)
		{
			rasterizerstate=Wireframestate;
		}
		else
		{
			rasterizerstate=Solidstate;
		}
	}

	void RenderWorld::UpdateScene(Level * level)
	{	
		
		StaticMeshList=std::move(level->StaticMeshList);
		
		sky = level->sky;
		for (uint32_t j = 0; j < level->DirectionalLightList.size(); j++)
		{
			m_constantBufferData[0]->directionallights[j] = *level->DirectionalLightList[j];
		}
		for (uint32_t j = 0; j < level->SpotLightList.size(); j++)
		{
			m_constantBufferData[0]->spotlights[j] = *level->SpotLightList[j];
			m_constantBufferData[0]->NumOfSpotLights= level->SpotLightList.size();
		}
		for (uint32_t j = 0; j < level->PointLightList.size(); j++)
		{
			m_constantBufferData[0]->pointlights[j] = *level->PointLightList[j];
			m_constantBufferData[0]->NumOfPointLights= level->PointLightList.size();
		}
		for (uint32_t j = 1; j < 9; j++)
		{
			m_constantBufferData[j] = m_constantBufferData[0];
		}
	}

	void RenderWorld::RenderScene()
	{
		BeginRender();
		RenderFrame();
		EndRender();
	}

	void RenderWorld::Initilize(HWND windows)
	{
		Renderer::renderdevice =new RenderDevice_DX11(windows, false, true);
		Renderer::shadermanager = new ShaderManager;
		Renderer::shadermanager->CreateShader();
		camera = new Camera;
		GPUBufferDesc constantdesc;
		constantdesc.BindFlags = BIND_CONSTANT_BUFFER;
		constantdesc.ByteWidth = sizeof(RenderConstantBuffer);
		constantdesc.CPUAccessFlags = 0;
		constantdesc.MiscFlags = 0;
		constantdesc.Usage = USAGE_DEFAULT;
		constantdesc.StructureByteStride = 0;
		constbuffer = new GPUBuffer;
		Renderer::GetDevice()->CreateBuffer(&constantdesc, nullptr, constbuffer);
#ifdef PREDEBUG
		Renderer::GetDevice()->SetName(constbuffer, "constantbuffer");
#endif // DEBUG

		RECT rect = RECT();
		GetClientRect(windows, &rect);

		int SCREENWIDTH = rect.right - rect.left;
		int SCREENHEIGHT = rect.bottom - rect.top;

		camera->SetPosition(0.0f, 2.0f, -15.0f);
		camera->SetLens(0.25f*MathHelper::Pi, (float)(SCREENWIDTH / SCREENHEIGHT), 0.01f, 1000.0f);

		mLastMousePos.x = 0;
		mLastMousePos.y = 0;

		
		JobScheduler::Initialize();
		m_constantBufferData = new RenderConstantBuffer*[9];
		for (uint32_t i = 0; i < 9; ++i)
		{
			m_constantBufferData[i] = new RenderConstantBuffer();
			
			Handle[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
		}
		
		Solidstate = new RasterizerState;
		Wireframestate = new RasterizerState;
		defalutDSS = new DepthStencilState;
		RasterizerStateDesc Wireframedesc;
		Wireframedesc.FillMode = FILL_WIREFRAME;
		Wireframedesc.CullMode = CULL_BACK;
		Wireframedesc.FrontCounterClockWise = false;
		Wireframedesc.DepthCilpEnable = true;

		RasterizerStateDesc Soliddesc;
		Wireframedesc.FillMode = FILL_SOLID;
		Wireframedesc.CullMode = CULL_BACK;
		Wireframedesc.FrontCounterClockWise = false;
		Wireframedesc.DepthCilpEnable = true;

		DepthStencilStateDesc depthstencildesc;
		depthstencildesc.DepthEnable = true;
		depthstencildesc.DepthWriteMask = DEPTH_WRITE_MASK_ALL;
		depthstencildesc.DepthFunc = COMPARSION_LESS;

		Renderer::GetDevice()->CreateDepthStencilState(&depthstencildesc, defalutDSS);
		Renderer::GetDevice()->CreateRasterizerState(&Soliddesc, Solidstate);
		Renderer::GetDevice()->CreateRasterizerState(&Wireframedesc, Wireframestate);
#ifdef PREDEBUG
		Renderer::GetDevice()->SetName(Solidstate, "Solidstate");
		Renderer::GetDevice()->SetName(Wireframestate, "Wireframestate");
		Renderer::GetDevice()->SetName(defalutDSS, "defalutDSS");
#endif // DEBUG
		SpLutSampler = new Sampler;
		SamplerDesc splutsampler;
		splutsampler.Filter = FILTER_MIN_MAG_MIP_LINEAR;
		splutsampler.AddressU = TEXTURE_ADDRESS_CLAMP;
		splutsampler.AddressV = TEXTURE_ADDRESS_CLAMP;
		splutsampler.AddressW = TEXTURE_ADDRESS_CLAMP;
		splutsampler.MaxAnisotropy = 1;
		Renderer::GetDevice()->CreateSamplerState(&splutsampler, SpLutSampler);
#ifdef PREDEBUG
		Renderer::GetDevice()->SetName(SpLutSampler, "SpLutSampler");
#endif // DEBUG
		tonemappingsampler = new Sampler;
		SamplerDesc tonemappingdesc;
		tonemappingdesc.Filter = FILTER_MIN_MAG_MIP_LINEAR;
		tonemappingdesc.AddressU = tonemappingdesc.AddressV = tonemappingdesc.AddressW = TEXTURE_ADDRESS_WRAP;
		tonemappingdesc.MaxAnisotropy = 1;
		Renderer::GetDevice()->CreateSamplerState(&tonemappingdesc, tonemappingsampler);
#ifdef PREDEBUG
		Renderer::GetDevice()->SetName(tonemappingsampler, "tonemappingsampler");
#endif // DEBUG

		shcoeffs COFS[9];

		/*TextureManager::GetLoader()->MakeIadiacneMap(COFS,nullptr,nullptr);

		for (uint32_t i = 0; i < 9; ++i)
		{
				m_constantBufferData[0]->COFS[i] = COFS[i];
				if (i == 8)
				{
					for (uint32_t i = 1; i < 9; ++i)
					{
						m_constantBufferData[i]->COFS[i] = m_constantBufferData[0]->COFS[0];
					}
				}
		}*/

		UINT screenwidth = Renderer::GetDevice()->GetScreenWidth();
		UINT screenheight = Renderer::GetDevice()->GetScreenHeight();
		FORMAT format = Renderer::GetDevice()->GetBackBufferFormat();
		UINT QUALITY = Renderer::GetDevice()->GetMSAAQUALITY();

		rendertarget = new RenderTarget(screenwidth,screenheight,true,format,1,4,QUALITY,false);
		
		

	}

	
}