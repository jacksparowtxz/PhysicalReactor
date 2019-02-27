#include "stdafx.h"
#include "Sky.h"
#include"MISC/AssetManager.h"

using namespace PRE;

Sky::Sky()
{


	std::vector<Vertex> vs;
	std::vector<uint32_t> is;
	CreateSphere(5000.0f, 30, 30, vs,is);
	SubMesh *sphere = new SubMesh(vs,is);
	



	SkyMesh = new StaticMesh;
	SkyMesh->Meshs.push_back(sphere);
	//AssetManager::GetDevice()->Import("Re/smsphere.obj",SkyMesh);
	SkyCubeMap = new Texture2D;
	TextureManager::GetLoader()->LoadTexture("Re/OutputCube1.dds", SkyCubeMap);
	Skymaterial = new Material("SkyMaterial");

	DepthStencilStateDesc dsdesc;
	dsdesc.DepthEnable = true;
	dsdesc.DepthFunc = COMPARSION_LESS_EQUAL;
	dsdesc.DepthWriteMask = DEPTH_WRITE_MASK_ALL;

	Skymaterial->SetDepthStencilState(dsdesc);
	Skymaterial->EmissiveMap = SkyCubeMap;

	SamplerDesc spdesc;
	spdesc.Filter = FILTER_MIN_MAG_MIP_LINEAR;
	spdesc.AddressU = TEXTURE_ADDRESS_WRAP;
	spdesc.AddressV = TEXTURE_ADDRESS_WRAP;

	Skymaterial->SetSampleState(spdesc);
	SkyMesh->Meshs[0]->material = Skymaterial;


	RasterizerStateDesc rsdesc;
	rsdesc.CullMode = CULL_NONE;
	rsdesc.DepthCilpEnable = true;
	Skymaterial->SetRasterState(rsdesc);
}


Sky::~Sky()
{
	delete SkyCubeMap;
	delete Skymaterial;
	delete SkyMesh;
}
void Sky::CreateSphere(float radius, UINT sliceCount, UINT stackCount, vector<Vertex>& Inverteices, vector<uint32_t>& indices)
{
	Inverteices.clear();
	indices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(XMFLOAT3(0.0f, +radius, 0.0f), XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	Vertex bottomVertex(XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	Inverteices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f*XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Vertex v;

			// spherical to cartesian
			v.Pos.x = radius * sinf(phi)*cosf(theta);
			v.Pos.y = radius * cosf(phi);
			v.Pos.z = radius * sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.Tangent.x = -radius * sinf(phi)*sinf(theta);
			v.Tangent.y = 0.0f;
			v.Tangent.z = +radius * sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.Tangent);
			XMStoreFloat3(&v.Tangent, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Pos);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexCoord.x = theta / XM_2PI;
			v.TexCoord.y = phi / XM_PI;

			Inverteices.push_back(v);
		}
	}

	Inverteices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)Inverteices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
}