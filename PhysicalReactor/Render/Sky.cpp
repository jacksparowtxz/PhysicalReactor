#include "stdafx.h"
#include "Sky.h"
#include"MISC/AssetManager.h"

using namespace PRE;

Sky::Sky()
{
	SkyMesh = new StaticMesh;
	Skymaterial = new Material("SkyMaterial");
	AssetManager::GetDevice()->Import("Re/smsphere.obj",SkyMesh);
	SkyCubeMap = new Texture2D;
	TextureManager::GetLoader()->LoadTexture("Re/DarkStorm4K.hdr", SkyCubeMap,true);//DarkStorm4K  SkyhighFluffycloudField4k
	
	EnvMap=new Texture2D;
	SpLutMap = new Texture2D;
	TextureManager::GetLoader()->MakeRadianceMap(SkyCubeMap,EnvMap,SpLutMap);
	
	DepthStencilStateDesc dsdesc;
	dsdesc.DepthEnable = true;
	dsdesc.DepthFunc = COMPARSION_LESS_EQUAL;
	dsdesc.DepthWriteMask = DEPTH_WRITE_MASK_ALL;
	Skymaterial->EmissiveMap = std::move(SkyCubeMap);
	Skymaterial->SetDepthStencilState(dsdesc);

	SamplerDesc spdesc;
	spdesc.Filter = FILTER_MIN_MAG_MIP_LINEAR;
	spdesc.AddressU = TEXTURE_ADDRESS_WRAP;
	spdesc.AddressV = TEXTURE_ADDRESS_WRAP;
	spdesc.MaxAnisotropy = 1;

	Skymaterial->SetEmissiveSampler(spdesc);
	SkyMesh->Meshs[0]->material =std::move(Skymaterial);


	RasterizerStateDesc rsdesc;
	rsdesc.CullMode = CULL_NONE;
	rsdesc.DepthCilpEnable = true;
	rsdesc.ScissorEnable = false;
	Skymaterial->SetRasterState(rsdesc);

	IradMap = new Texture2D;
	TextureManager::GetLoader()->MakeIadiacneMap(nullptr,EnvMap,IradMap);
}


Sky::~Sky()
{
	SAFE_DELETE(SkyMesh);
	SAFE_DELETE(EnvMap);
	SAFE_DELETE(SpLutMap);
	SAFE_DELETE(IradMap);
}
