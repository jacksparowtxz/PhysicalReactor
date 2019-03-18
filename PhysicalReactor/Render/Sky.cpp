#include "stdafx.h"
#include "Sky.h"
#include"MISC/AssetManager.h"

using namespace PRE;

Sky::Sky()
{
	SkyMesh = new StaticMesh;
	AssetManager::GetDevice()->Import("Re/smsphere.obj",SkyMesh);
	SkyCubeMap = new Texture2D;
	TextureManager::GetLoader()->LoadTexture("Re/SkyhighFluffycloudField4k.hdr", SkyCubeMap,true);
	Skymaterial = new Material("SkyMaterial");
	Texture2D* EnvMap=new Texture2D;
	Texture2D* SpLutMap = new Texture2D;
	TextureManager::GetLoader()->MakeRadianceMap(SkyCubeMap,EnvMap,SpLutMap);

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
	rsdesc.ScissorEnable = false;
	Skymaterial->SetRasterState(rsdesc);
}


Sky::~Sky()
{
	delete SkyCubeMap;
	delete Skymaterial;
	delete SkyMesh;
	delete EnvMap;
	delete SpLutMap;
}
