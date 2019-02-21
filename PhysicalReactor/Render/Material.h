#pragma once
#include"Render/GraphicEnum.h"
#include"Render/GraphicsResource.h"
#include<string>

using namespace PRE;
using namespace std;



class Material
{
public:

	Material();
	Material(const std::string &newName);
	~Material();
	void Init();



	RENDERTYPE GetRenderType() const;
	SHADINGMODEL GetShadingMode() const;
	BLENDMODE GetBlendMode() const;

	


	RENDERTYPE MaterialType;
	SHADINGMODEL Shadingmodel;
	BLENDMODE BlendMode;
	DecalBlendMode decalblendmode;
	Tessellation tessellation;
	string Materialname;

	Texture2D* BaseColorMap;
	string BaseColorMapName;

	Texture2D* MetalicMap;
	string MetalicMapName;

	Texture2D* SpecularMap;
	string SpecularMapName;

	Texture2D* RoughnessMap;
	string RoughnessMapName;

	Texture2D* EmissiveMap;
	string EmissiveMapName;

	Texture2D* OpacityMap;
	string OpacityMapName;

	Texture2D* OpcaityMaskMap;
	string OpcaityMaskMapName;

	Texture2D* NormalMap;
	string NormalMapName;

	Texture2D* WorldPositionOffset;
	string WorldPositionOffsetName;

	Texture2D* WorldDisplacement;
	string WorldDisplacementName;
	
	Texture2D* TessellationMultiplerMap;
	string TessellationMultipler;
	
	Texture2D* SubsurfaceMap;
	string SubsurfaceMapName;
	
	Texture2D* AmbientMap;
	string AmbientMapname;
	
	Texture2D* RefractionMap;
	string RefractionMapname;

	Texture2D* PixelDepthOffset;
	string PixelDepthOffsetName;
	
	float Metalness;
	float Roughness;
	XMFLOAT3 BaseColor;

	bool TwoSide;


};
