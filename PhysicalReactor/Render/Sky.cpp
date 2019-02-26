#include "stdafx.h"
#include "Sky.h"
#include"MISC/AssetManager.h"


Sky::Sky()
{
	SkyMesh = new StaticMesh;
	AssetManager::GetDevice()->Import("Re/smsphere.h",SkyMesh);
	SkyCubeMap = new Texture2D;
	TextureManager::GetLoader()->LoadTexture("Re/OutCube1.dds", SkyCubeMap);
	Skymaterial = new Material("SkyMaterial");

	DepthStencilStateDesc dsdesc;
	dsdesc.DepthFunc = COMPARSION_LESS_EQUAL;

	Skymaterial->SetDepthStencilState(dsdesc);
	Skymaterial->EmissiveMap = SkyCubeMap;

}


Sky::~Sky()
{
	delete SkyCubeMap;
	delete Skymaterial;
	delete SkyMesh;
}
