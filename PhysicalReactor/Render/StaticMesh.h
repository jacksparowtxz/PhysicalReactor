#pragma once
#include"Vertex.h"
#include"Material.h"
#include"Render/GraphicsResource.h"
#include"Render/GraphicsDescriptors.h"
#include"Render/Renderer.h"
#include"MISC/Intersectables.h"
#include<vector>

using namespace PRE;
using namespace std;


struct SubMesh
{

	vector<Vertex> vertices;
	vector<uint32_t> Indices;
	
	GPUBuffer* mVertexBuffer;
	GPUBuffer* mIndexBuffer;
	Material* material;
	SubMesh()
	{
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;
		material = nullptr;
	}
	SubMesh(vector<Vertex> Inverteices,vector<uint32_t> indices)
	{
		vertices = Inverteices;
		Indices = indices;
	    

		GPUBufferDesc vertexdesc;
		vertexdesc.BindFlags = BIND_VERTEX_BUFFER;
		vertexdesc.ByteWidth = sizeof(Vertex)*vertices.size();
		vertexdesc.CPUAccessFlags = 0;
		vertexdesc.Usage = USAGE_IMMUTABLE;
		vertexdesc.MiscFlags = 0;
		vertexdesc.StructureByteStride = 0;
		
		SubresourceData subdata = {};
		subdata.pSysMem = vertices.data();
		mVertexBuffer = new GPUBuffer;
		Renderer::GetDevice()->CreateBuffer(&vertexdesc, &subdata, mVertexBuffer);

		GPUBufferDesc IndexBuffer;
		IndexBuffer.BindFlags = BIND_INDEX_BUFFER;
		IndexBuffer.ByteWidth = sizeof(Indices)*Indices.size();
		IndexBuffer.CPUAccessFlags = 0;
		IndexBuffer.MiscFlags = 0;
		IndexBuffer.StructureByteStride = 0;
		IndexBuffer.Usage = USAGE_IMMUTABLE;

		SubresourceData subdata;
		subdata.pSysMem = Indices.data();
		mIndexBuffer = new GPUBuffer;
		Renderer::GetDevice()->CreateBuffer(&IndexBuffer, &subdata, mIndexBuffer);


	}
	~SubMesh()
	{
		SAFE_DELETE(material);
		SAFE_DELETE(mVertexBuffer);
		SAFE_DELETE(mIndexBuffer);
	}

};


class StaticMesh
{
public:
	string name;
	string directory;
	AABB* aabb;
	vector<SubMesh*> Meshs;
	

	StaticMesh();
	~StaticMesh();
};

