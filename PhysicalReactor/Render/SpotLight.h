#pragma once
#include "Light.h"
class SpotLight
{
public:
	SpotLight();
	~SpotLight();
	//////////////////////////////SpotLight
	XMFLOAT3 color;
	float OuterConeAngle;
	XMFLOAT3 position;
	float Intensity;
	XMFLOAT3 Rotaion;
	float InnerConeAngle;

	float attenuationradius;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
	

	
/*	float lightfalloffexponent;
	float IESIntensityScale;
	IntensityUnits intensityunits;
	///////////////////////spoint  Light Profile
	//IES Texture;
	
	bool IESIntensity;
	bool  useInverseSquaredFalloff;
	bool CastShadowformCinematicobjectonly;*/

};

int i = sizeof(SpotLight);