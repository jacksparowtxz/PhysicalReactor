#pragma once
#include"Render/Renderer.h"
#include"Render/Renderable.h"
#include"Render/Material.h"
#include"MISC/TextureManager.h"



using namespace PRE;

class Sky
{
public:
	Sky();
	~Sky();
	void CreateSphere(float radius, UINT sliceCount, UINT stackCount, vector<Vertex>& Inverteices, vector<uint32_t>& indices);
	StaticMesh* SkyMesh;
	Material* Skymaterial;
	Texture2D* SkyCubeMap;
};

