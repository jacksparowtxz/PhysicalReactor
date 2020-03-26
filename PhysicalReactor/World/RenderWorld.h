#pragma once
#include "Render/GraphicsResource.h"
#include "World/Level.h"
#include "Render/Renderable.h"
#include "Render/Camera.h"
#include "Render/DrawKey.h"
#include "MISC/MiscHelper.h"
#include "Render/sky.h"
#include<windows.h>
#include<vector>

using namespace std;
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
			DirectX::XMStoreFloat4x4(&WorldViewProj, XMMatrixIdentity());

			DirectX::XMStoreFloat4x4(&model, XMMatrixIdentity());
		}

		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 textransform;
		DirectX::XMFLOAT4X4 worldinvtranspose;

		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 pading;
		DirectX::XMFLOAT4X4 pading1;
		DirectX::XMFLOAT4X4 pading2;

		DirectX::XMFLOAT4X4 WorldViewProj;
		DirectX::XMFLOAT4X4 pading3;
		DirectX::XMFLOAT4X4 pading4;
		DirectX::XMFLOAT4X4 pading5;

		DirectionalLight directionallights[4];//equal4x4
		PointLight pointlights[4];//equal 4x4
		SpotLight spotlights[4];//equal 4x4
		
		SphereAreaLight spherearealight[4];
		DiscLight disclight[4];
		TubeLight tubelight[4];
		RectangleLight rectanglelight[2];
		DirectX::XMFLOAT4 EyePos;
		XMFLOAT3 BaseColorFactor;
		int NumOfSpotLights;
		float emissive_factor;
		float metalic_factor;
		float Roughness_factor;
		int NumOfPointLights;
		int NumOfSphereLights;
		int NumOfDiscLights;
		int NumOfTubeLights;
		int NumOfRectangleLights;
	};

	
	class RenderWorld
	{
	public:
		RenderWorld(HWND windows);
		~RenderWorld();
		void BeginRender();
		void RenderFrame();
		void EndRender();
		void Update(double deltatime);
		void RenderScene();
		void Initilize(HWND windows);
		void ReSize(int width, int height);
		void MoveForWard(float Direction);
		void MoveRight(float Direction);
		void CameraRotation(WPARAM btnState, int x, int y);
		void SetMousePosition(HWND windows, int x, int y);
		void RenderMaterial(SHADERSTAGE stage, SubMesh* submesh);
		void InitViews(Level* currentlevel);
		void RenderWireframe(bool Wireframe);
		void UpdateScene(Level* level);
		
		vector<StaticMesh*> StaticMeshList;

		vector<StaticMesh> VisiblityMesh;
		vector<StaticMesh> TVisiblityMesh;
		vector<PointLight*> PointLights;
		vector<SpotLight*> SpotLights;
		vector<DirectionalLight*> DirectionalLights;
		vector<SkyLight*> SkyLights;
		

		RenderConstantBuffer** m_constantBufferData=nullptr;
		GPUBuffer* constbuffer;
		Camera *camera;
		POINT mLastMousePos;
		double dt;
		HANDLE Handle[9];
		uint64_t ListPtr[9];
		uint64_t TListPtr[9];
		RasterizerState* Wireframestate;
		RasterizerState* Solidstate;
		RasterizerState* rasterizerstate;
		DepthStencilState *defalutDSS;

		RenderTarget* rendertarget;
	    
		Sky* sky;
		Sampler* SpLutSampler;
		Sampler* tonemappingsampler;
		GPUResource* Nullresource;

	};
}