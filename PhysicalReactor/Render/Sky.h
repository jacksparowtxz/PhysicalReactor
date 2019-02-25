#pragma once
#include"Render/Renderer.h"
#include"Render/Renderable.h"
#include"Render/Material.h"




using namespace PRE;

class Sky
{
public:
	Sky();
	~Sky();

	StaticMesh* SkyMesh;
	Material* Skymaterial;
};

