#pragma once
#include"Render/GraphicEnum.h"
#include"Render/GraphicsResource.h"
#include"MISC/MiscHelper.h"
#include<string>

using namespace PRE;
using namespace std;



class Material
{
public:

	Material();
	Material(const std::string &newName);
	~Material();
	void Init(const std::string &newName);



	RENDERTYPE GetRenderType() const;
	SHADINGMODEL GetShadingMode() const;
	BLENDMODE GetBlendMode() const;
	void SetRasterState(RasterizerStateDesc rsdesc);
	void SetDepthStencilState(DepthStencilStateDesc dsdesc);
	void SetSampleState(SamplerDesc spdesc);
	


	RENDERTYPE MaterialType;
	SHADINGMODEL Shadingmodel;
	BLENDMODE BlendMode;
	DecalBlendMode decalblendmode;
	Tessellation tessellation;
	string Materialname;

	Texture2D* BaseColorMap;
	string BaseColorMapName;
	Sampler* BaseColorSampler;

	Texture2D* MetalicMap;
	string MetalicMapName;
	Sampler* MetalicSampler;

	Texture2D* SpecularMap;
	string SpecularMapName;
	Sampler* SpecularSampler;

	Texture2D* RoughnessMap;
	string RoughnessMapName;
	Sampler* RoughnessSampler;

	Texture2D* EmissiveMap;
	string EmissiveMapName;
	Sampler* EmissiveSampler;

	Texture2D* OpacityMap;
	string OpacityMapName;
	Sampler* OpacitySampler;

	Texture2D* OpcaityMaskMap;
	string OpcaityMaskMapName;
	Sampler* OpcaityMaskSampler;

	Texture2D* NormalMap;
	string NormalMapName;
	Sampler* NormalSampler;

	Texture2D* WorldPositionOffset;
	string WorldPositionOffsetName;
	Sampler* WorldPositionOffsetSampler;

	Texture2D* WorldDisplacement;
	string WorldDisplacementName;
	Sampler* WorldDisplacementSampler;
	
	Texture2D* TessellationMultiplerMap;
	string TessellationMultipler;
	Sampler* TessellationMultiplerSampler;
	
	Texture2D* SubsurfaceMap;
	string SubsurfaceMapName;
	Sampler* SubsurfaceSampler;
	
	Texture2D* AmbientMap;
	string AmbientMapname;
	Sampler* AmbientSampler;
	
	Texture2D* RefractionMap;
	string RefractionMapname;
	Sampler* RefractionSampler;

	Texture2D* PixelDepthOffset;
	string PixelDepthOffsetName;
	Sampler* PixelDepthOffsetSampler;

	Sampler* InitiSampler;
	RasterizerState* rasterzerstate;
	DepthStencilState* depthstencilstate;


	float Metalness;
	float Roughness;
	XMFLOAT3 BaseColor;

	bool TwoSide;

	BitSizeStruct<31> *MaterialID;
};
