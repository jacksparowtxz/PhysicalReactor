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

	StaticMesh* SkyMesh;
	Material* Skymaterial;
	Texture2D* SkyCubeMap;
};
