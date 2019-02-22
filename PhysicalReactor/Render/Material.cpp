#include "stdafx.h"
#include "Material.h"
#include "Render/Renderer.h"

Material::Material()
{
	Init();
}


Material::~Material()
{
	delete InitiSampler;
}

Material::Material(const std::string &newName)
{
	Init();
	Materialname = newName;
}


void Material::Init()
{
	MaterialType = RENDERTYPE::RENDERTYPE_SURFACE;
	Shadingmodel = SHADINGMODEL::SHADINGMODEL_DEFAULTUNLIT;
	BlendMode = BLENDMODE::BLEND_OPAQUE;

	Materialname = "NewMaterial";

	SamplerDesc InitSamplerDesc;
	InitiSampler = new Sampler;
	Renderer::GetDevice()->CreateSamplerState(&InitSamplerDesc, InitiSampler);
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



