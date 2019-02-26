#include "stdafx.h"
#include "Sky.h"
#include"MISC/AssetManager.h"


Sky::Sky()
{
	SkyMesh = new StaticMesh;
	AssetManager::GetDevice()->Import("Re/smsphere.obj",SkyMesh);
	SkyCubeMap = new Texture2D;
	TextureManager::GetLoader()->LoadTexture("Re/OutputCube1.dds", SkyCubeMap);
	Skymaterial = new Material("SkyMaterial");

	DepthStencilStateDesc dsdesc;
	dsdesc.DepthFunc = COMPARSION_LESS_EQUAL;

	Skymaterial->SetDepthStencilState(dsdesc);
	Skymaterial->EmissiveMap = SkyCubeMap;

	SamplerDesc spdesc;
	spdesc.Filter = FILTER_MIN_MAG_MIP_LINEAR;
	spdesc.AddressU = TEXTURE_ADDRESS_WRAP;
	spdesc.AddressV = TEXTURE_ADDRESS_WRAP;

	Skymaterial->SetSampleState(spdesc);


}


Sky::~Sky()
{
	delete SkyCubeMap;
	delete Skymaterial;
	delete SkyMesh;
}
