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
	
	SAFE_DELETE(MaterialID);
	SAFE_DELETE(rasterzerstate);
	SAFE_DELETE(depthstencilstate);

	

	SAFE_DELETE(BaseColorMap);
	SAFE_DELETE(MetalicMap);
	SAFE_DELETE(SpecularMap);
	SAFE_DELETE(RoughnessMap);
	SAFE_DELETE(EmissiveMap);
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

	SAFE_DELETE(BaseColorSampler);
	SAFE_DELETE(MetalicSampler);
	SAFE_DELETE(SpecularSampler);
	SAFE_DELETE(RoughnessSampler);
	SAFE_DELETE(EmissiveSampler);
	SAFE_DELETE(OpacitySampler);
	SAFE_DELETE(OpcaityMaskSampler);
	SAFE_DELETE(NormalSampler);
	SAFE_DELETE(WorldPositionOffsetSampler);
	SAFE_DELETE(WorldDisplacementSampler);
	SAFE_DELETE(TessellationMultiplerSampler);
	SAFE_DELETE(SubsurfaceSampler);
	SAFE_DELETE(AmbientSampler);
	SAFE_DELETE(RefractionSampler);
	SAFE_DELETE(PixelDepthOffsetSampler);
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



void Material::SetBaseColorSampler(SamplerDesc spdesc)
{
	if (BaseColorSampler==nullptr)
	{
		BaseColorSampler = new Sampler;
	}
	
	Renderer::GetDevice()->DestroySamplerState(BaseColorSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, BaseColorSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(BaseColorSampler, "BaseColorSampler");
#endif // DEBUG
}

void Material::SetMetalicSampler(SamplerDesc spdesc)
{
	if (MetalicSampler == nullptr)
	{
		MetalicSampler = new Sampler;
	}

	Renderer::GetDevice()->DestroySamplerState(MetalicSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, MetalicSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(MetalicSampler, "MetalicSampler");
#endif // DEBUG
	
	
}

void Material::SetSpecularSampler(SamplerDesc spdesc)
{
	if (SpecularSampler ==nullptr )
	{
		SpecularSampler = new Sampler;
	}

	Renderer::GetDevice()->DestroySamplerState(SpecularSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, SpecularSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(SpecularSampler, "SpecularSampler");
#endif // DEBUG
	

	

}

void Material::SetRoughnessSampler(SamplerDesc spdesc)
{
	if (RoughnessSampler == nullptr)
	{
		RoughnessSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(RoughnessSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, RoughnessSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(RoughnessSampler, "RoughnessSampler");
#endif // DEBUG
}

void Material::SetEmissiveSampler(SamplerDesc spdesc)
{
	if (EmissiveSampler == nullptr)
	{
		EmissiveSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(EmissiveSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, EmissiveSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(EmissiveSampler, "EmissiveSampler");
#endif // DEBUG
}

void Material::SetOpacitySampler(SamplerDesc spdesc)
{
	if (OpacitySampler == nullptr)
	{
		OpacitySampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(OpacitySampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, OpacitySampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(OpacitySampler, "OpacitySampler");
#endif // DEBUG
}

void Material::SetOpcaityMaskSampler(SamplerDesc spdesc)
{
	if (OpcaityMaskSampler == nullptr)
	{
		OpcaityMaskSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(OpcaityMaskSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, OpcaityMaskSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(OpcaityMaskSampler, "OpcaityMaskSampler");
#endif // DEBUG

}

void Material::SetNormalSampler(SamplerDesc spdesc)
{
	if (NormalSampler == nullptr)
	{
		NormalSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(NormalSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, NormalSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(NormalSampler, "NormalSampler");
#endif // DEBUG
	
	
	
}

void Material::SetWorldPositionOffsetSampler(SamplerDesc spdesc)
{
	if (WorldPositionOffsetSampler == nullptr)
	{
		WorldPositionOffsetSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(WorldPositionOffsetSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, WorldPositionOffsetSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(WorldPositionOffsetSampler, "WorldPositionOffsetSampler");
#endif // DEBUG
}

void Material::SetWorldDisplacementSampler(SamplerDesc spdesc)
{
	if (WorldDisplacementSampler == nullptr)
	{
		WorldDisplacementSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(WorldDisplacementSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, WorldDisplacementSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(WorldDisplacementSampler, "WorldDisplacementSampler");
#endif // DEBUG
	
	

}

void Material::SetTessellationMultiplerSampler(SamplerDesc spdesc)
{
	if (TessellationMultiplerSampler == nullptr)
	{
		TessellationMultiplerSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(TessellationMultiplerSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, TessellationMultiplerSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(TessellationMultiplerSampler, "TessellationMultiplerSampler");
#endif // DEBUG
}

void Material::SetSubsurfaceSampler(SamplerDesc spdesc)
{
	if (SubsurfaceSampler == nullptr)
	{
		SubsurfaceSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(SubsurfaceSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, SubsurfaceSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(SubsurfaceSampler, "SubsurfaceSampler");
#endif // DEBUG

}

void Material::SetAmbientSampler(SamplerDesc spdesc)
{
	if (AmbientSampler == nullptr)
	{
		AmbientSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(AmbientSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, AmbientSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(AmbientSampler, "AmbientSampler");
#endif // DEBUG
}

void Material::SetRefractionSampler(SamplerDesc spdesc)
{
	if (RefractionSampler == nullptr)
	{
		RefractionSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(RefractionSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, RefractionSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(RefractionSampler, "RefractionSampler");
#endif // DEBUG
}

void Material::SetPixelDepthOffsetSampler(SamplerDesc spdesc)
{
	if (PixelDepthOffsetSampler == nullptr)
	{
		PixelDepthOffsetSampler = new Sampler;
	}
	Renderer::GetDevice()->DestroySamplerState(PixelDepthOffsetSampler);
	Renderer::GetDevice()->CreateSamplerState(&spdesc, PixelDepthOffsetSampler);
#ifdef PREDEBUG
	Renderer::GetDevice()->SetName(PixelDepthOffsetSampler, "PixelDepthOffsetSampler");
#endif // DEBUG
}




