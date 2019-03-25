#include "stdafx.h"
#include "Material.h"
#include "Render/Renderer.h"

Material::Material()
{
	Materialname = "NewMaterial";
	Init(Materialname);
}


Material::~Material()
{
	SAFE_DELETE(InitiSampler);
	SAFE_DELETE(MaterialID);
	SAFE_DELETE(rasterzerstate);
	SAFE_DELETE(depthstencilstate);
	SAFE_DELETE(BaseColorMap);
	SAFE_DELETE(MetalicMap);
	SAFE_DELETE(SpecularMap);
	SAFE_DELETE(RoughnessMap);
	//SAFE_DELETE(EmissiveMap);
	SAFE_DELETE(OpacityMap);
	SAFE_DELETE(OpcaityMaskMap);
	SAFE_DELETE(NormalMap);
	SAFE_DELETE(WorldPositionOffset);
	SAFE_DELETE(WorldDisplacement);
	SAFE_DELETE(TessellationMultiplerMap);
	SAFE_DELETE(SubsurfaceMap);
	SAFE_DELETE(AmbientMap);
	SAFE_DELETE(RefractionMap);
	SAFE_DELETE(PixelDepthOffset);
	SAFE_DELETE(SubsurfaceMap);
}

Material::Material(const std::string &newName)
{
	Init(newName);
}


void Material::Init(const std::string &newName)
{
	MaterialType = RENDERTYPE::RENDERTYPE_SURFACE;
	Shadingmodel = SHADINGMODEL::SHADINGMODEL_DEFAULTUNLIT;
	BlendMode = BLENDMODE::BLEND_OPAQUE;

	Materialname = "NewMaterial";

	SamplerDesc InitSamplerDesc;
	InitiSampler = new Sampler;
	InitSamplerDesc.Filter = FILTER_ANISOTROPIC;
	InitSamplerDesc.AddressU = TEXTURE_ADDRESS_WRAP;
	InitSamplerDesc.AddressV = TEXTURE_ADDRESS_WRAP;
	InitSamplerDesc.AddressW = TEXTURE_ADDRESS_WRAP;
	Renderer::GetDevice()->CreateSamplerState(&InitSamplerDesc, InitiSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(InitiSampler, "InitiSampler");
#endif // DEBUG
	RasterizerStateDesc RSdesc;
	rasterzerstate = new RasterizerState;
	Renderer::GetDevice()->CreateRasterizerState(&RSdesc,rasterzerstate);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(rasterzerstate, "rasterzerstate");
#endif // DEBUG
	DepthStencilStateDesc DSdesc;
	depthstencilstate = new DepthStencilState;
	Renderer::GetDevice()->CreateDepthStencilState(&DSdesc,depthstencilstate);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(depthstencilstate, "depthstencilstate");
#endif // DEBUG

	BaseColorMap=nullptr;
	BaseColorMapName="";
	BaseColorSampler = InitiSampler;

	MetalicMap=nullptr;
	MetalicMapName="";
	MetalicSampler = InitiSampler;

	SpecularMap=nullptr;
	SpecularMapName="";
	SpecularSampler = InitiSampler;

	RoughnessMap=nullptr;
	RoughnessMapName="";
	RoughnessSampler = InitiSampler;

	EmissiveMap=nullptr;
	EmissiveMapName="";
	EmissiveSampler = InitiSampler;

	OpacityMap=nullptr;
	OpacityMapName="";
	OpacitySampler = InitiSampler;

	OpcaityMaskMap=nullptr;
	OpcaityMaskMapName="";
	OpcaityMaskSampler = InitiSampler;

	NormalMap=nullptr;
	NormalMapName="";
	NormalSampler = InitiSampler;

	WorldPositionOffset=nullptr;
	WorldPositionOffsetName="";
	WorldPositionOffsetSampler = InitiSampler;

	WorldDisplacement=nullptr;
	WorldDisplacementName="";
	WorldDisplacementSampler = InitiSampler;

	TessellationMultiplerMap=nullptr;
	TessellationMultipler="";
	TessellationMultiplerSampler = InitiSampler;

	SubsurfaceMap=nullptr;
	SubsurfaceMapName="";
	SubsurfaceSampler = InitiSampler;

	AmbientMap=nullptr;
	AmbientMapname="";
	AmbientSampler = InitiSampler;

	RefractionMap=nullptr;
	RefractionMapname="";
	RefractionSampler = InitiSampler;

	PixelDepthOffset=nullptr;
	PixelDepthOffsetName="";
	PixelDepthOffsetSampler = InitiSampler;

	TwoSide=false;

	
	Materialname = newName;
	uint64_t ID = std::hash<std::string>{}(newName);
	MaterialID = new BitSizeStruct<31>();
	MaterialID->data = std::move(ID);
}

RENDERTYPE Material::GetRenderType() const
{
	 
	return RENDERTYPE::RENDERTYPE_SURFACE;

}

SHADINGMODEL Material::GetShadingMode() const
{

	return SHADINGMODEL::SHADINGMODEL_DEFAULTUNLIT;
}

BLENDMODE Material::GetBlendMode() const
{

	return BLENDMODE::BLEND_OPAQUE;
}

void Material::SetRasterState(RasterizerStateDesc rsdesc)
{
	Renderer::GetDevice()->DestroyRasterizerState(rasterzerstate);
	Renderer::GetDevice()->CreateRasterizerState(&rsdesc, rasterzerstate);

}

void Material::SetDepthStencilState(DepthStencilStateDesc dsdesc)
{
	Renderer::GetDevice()->DestroyDepthStencilState(depthstencilstate);
	Renderer::GetDevice()->CreateDepthStencilState(&dsdesc, depthstencilstate);

}

void Material::SetSampleState(SamplerDesc spdesc)
{
	Renderer::GetDevice()->DestroySamplerState(InitiSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, InitiSampler);
	EmissiveSampler = InitiSampler;
}



