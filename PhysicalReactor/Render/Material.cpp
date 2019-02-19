#include "stdafx.h"
#include "Material.h"


Material::Material()
{
	Init();
}


Material::~Material()
{
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

	 BaseColorMap=nullptr;
	 BaseColorMapName="";

	MetalicMap=nullptr;
	MetalicMapName="";

	SpecularMap=nullptr;
	SpecularMapName="";

	RoughnessMap=nullptr;
	RoughnessMapName="";

	EmissiveMap=nullptr;
	EmissiveMapName="";

	OpacityMap=nullptr;
	OpacityMapName="";

	OpcaityMaskMap=nullptr;
	OpcaityMaskMapName="";

	NormalMap=nullptr;
	NormalMapName="";

	WorldPositionOffset=nullptr;
	WorldPositionOffsetName="";

	WorldDisplacement=nullptr;
	WorldDisplacementName="";

	TessellationMultiplerMap=nullptr;
	TessellationMultipler="";

	SubsurfaceMap=nullptr;
	SubsurfaceMapName="";

	AmbientMap=nullptr;
	AmbientMapname="";

	RefractionMap=nullptr;
	RefractionMapname="";

	PixelDepthOffset=nullptr;
	PixelDepthOffsetName="";

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



