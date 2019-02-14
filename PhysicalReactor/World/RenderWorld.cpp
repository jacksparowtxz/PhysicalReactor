#include "World/RenderWorld.h"
#include "Render/Renderer.h"
#include "Render/RenderDevice_DX11.h"



namespace PRE
{
	RenderWorld::RenderWorld(HWND windows, Allocator* allocator) :StaticmeshList(*allocator)//:mVertexBuffer(0),mIndexBuffer(0),vs(0),ps(0),pinputlayout(0),constbuffer(0),pso(0)
	{
		Renderer::renderdevice = new RenderDevice_DX11(windows,false,true);
		/*GPUBufferDesc vertexdesc;
	    VertexPositionColor cubeVertices[] =
		{
			{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
			{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
			{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
			{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
			{XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
			{XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
			{XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
			{XMFLOAT3(0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
		};

		vertexdesc.BindFlags = BIND_VERTEX_BUFFER;
		vertexdesc.ByteWidth = sizeof(cubeVertices);
		vertexdesc.CPUAccessFlags = 0;
		vertexdesc.Usage = USAGE_IMMUTABLE;
		vertexdesc.MiscFlags = 0;
		vertexdesc.StructureByteStride = 0;
		SubresourceData subdata = {};
		subdata.pSysMem =cubeVertices;
		mVertexBuffer = new GPUBuffer;
		Renderer::GetDevice()->CreateBuffer(&vertexdesc, &subdata, mVertexBuffer);

	     unsigned short cubeIndices[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		 GPUBufferDesc IndexBuffer;
		 IndexBuffer.BindFlags = BIND_INDEX_BUFFER;
		 IndexBuffer.ByteWidth = sizeof(cubeIndices);
		 IndexBuffer.CPUAccessFlags = 0;
		 IndexBuffer.MiscFlags = 0;
		 IndexBuffer.StructureByteStride = 0;
		 IndexBuffer.Usage = USAGE_IMMUTABLE;

		 SubresourceData Isubdata;
		 Isubdata.pSysMem = cubeIndices;
		 mIndexBuffer = new GPUBuffer;
		 Renderer::GetDevice()->CreateBuffer(&IndexBuffer, &Isubdata, mIndexBuffer);
		 GraphicBlob* vsblob = new GraphicBlob;
		  const char* pszMultiByte = "vs.hlsl";  
		 int iSize;
		 wchar_t* pwszUnicode;

		 iSize = MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, NULL, 0);
		 pwszUnicode = (wchar_t *)malloc(iSize * sizeof(wchar_t)); 
		 MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, pwszUnicode, iSize);
		 vs = new VertexShader;
		 Renderer::GetDevice()->CreateVertexShader(pwszUnicode,vsblob,vs);

		 GraphicBlob* psblob = new GraphicBlob;
		 const char* ppszMultiByte = "ps.hlsl";
		 int piSize;
		 wchar_t* ppwszUnicode;

		 piSize = MultiByteToWideChar(CP_ACP, 0, ppszMultiByte, -1, NULL, 0);
		 ppwszUnicode = (wchar_t *)malloc(iSize * sizeof(wchar_t));
		 MultiByteToWideChar(CP_ACP, 0, ppszMultiByte, -1, ppwszUnicode, piSize);
		 ps = new PixelShader;
		 Renderer::GetDevice()->CreatePixelShader(ppwszUnicode, psblob, ps);


		 const VertexLayoutDesc vertexDesc[] =
		 {
			 { "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_PER_VERTEX_DATA, 0 },
			 { "COLOR", 0, FORMAT_R32G32B32_FLOAT, 0, 12, INPUT_PER_VERTEX_DATA, 0 },
		 };
		 pinputlayout = new VertexLayout;
		 Renderer::GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc),vsblob, pinputlayout);

		 GPUBufferDesc constantdesc;
		 constantdesc.BindFlags = BIND_CONSTANT_BUFFER;
		 constantdesc.ByteWidth = sizeof(ModelViewProjectionConstantBuffer);
		 constantdesc.CPUAccessFlags = 0;
		 constantdesc.MiscFlags = 0;
		 constantdesc.Usage = USAGE_DEFAULT;
		 constantdesc.StructureByteStride = 0;
		 constbuffer = new GPUBuffer;
		 Renderer::GetDevice()->CreateBuffer(&constantdesc, nullptr, constbuffer);
		 pso = new GraphicPSO;
		 pso->desc.vs = vs;
		 pso->desc.ps = ps;
		 pso->desc.VL = pinputlayout;


		 float aspectRatio = 1920.f / 1080.f;
		 float fovAngleY = 70.0f * XM_PI / 180.0f;

		 XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
			 fovAngleY,
			 aspectRatio,
			 0.01f,
			 100.0f
		 );

		 static const XMFLOAT4X4 Rotation0(
			 1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 1.0f, 0.0f,
			 0.0f, 0.0f, 0.0f, 1.0f
		 );

		 XMMATRIX orientationMatrix = XMLoadFloat4x4(&Rotation0);

		 XMStoreFloat4x4(
			 &m_constantBufferData.projection,
			 XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		 );

		 static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
		 static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
		 static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

		 XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
		 XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(90.f)));*/
	}


	void RenderWorld::BeginRender()
	{
	
		Renderer::GetDevice()->PresentBegin();
	
	
	}


	void RenderWorld::RenderFrame()
	{
	   
		/*Renderer::GetDevice()->SetResolution(1920, 1080);
		Renderer::GetDevice()->UpdateBuffer(constbuffer, &m_constantBufferData);
		Renderer::GetDevice()->BindGraphicsPSO(pso);





		Renderer::GetDevice()->BindConstantBuffer(pso, constbuffer);
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		Renderer::GetDevice()->BindVertexBuffers(&mVertexBuffer, 0, 1, &stride, &offset);
		Renderer::GetDevice()->BindIndexBuffer(mIndexBuffer, INDEXBUFFER_16BIT, 0);
		Renderer::GetDevice()->DrawIndexed(36, 0, 0);*/

	}

	void RenderWorld::EndRender()
	{
		/*Renderer::GetDevice()->FinishComanlist();
		Renderer::GetDevice()->ExcuteDeferredContexts();
		Renderer::GetDevice()->PresentEnd();*/
	}

	void RenderWorld::Update()
	{
		/*BeginRender();
		RenderFrame();
		EndRender();*/
	}

}