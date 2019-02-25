#pragma once
#include "Light.h"
class SkyLight :
	public Light
{
public:
	SkyLight();
	~SkyLight();

	/////////////////skylight
	SkySourceType skysourcetype;
	//CubeMap* skymap;
	float SourceCubemapangle;
	float cubemapResolution;
	float skyDistanceTHreadhold;
	bool CaptureEmissiveOnly;
	bool lowerhemisphereissoildColor;
	XMFLOAT4 *Lowerhemispherecolor;





	/////////////////////////Distance field 
	float occlusionmaxdistance;
	float occlusioncontance;
	float occlusionexponent;
	float minocclusion;
	XMFLOAT4* occlusiontint;
};

