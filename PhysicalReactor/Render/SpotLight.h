#pragma once
#include "Light.h"
class SpotLight :
	public Light
{
public:
	SpotLight();
	~SpotLight();
	//////////////////////////////SpotLight
	float InnerConeAngle;
	float OuterConeAngle;
	
	float attenuationradius;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
	float lightfalloffexponent;
	bool  useInverseSquaredFalloff;
	bool CastShadowformCinematicobjectonly;
	IntensityUnits intensityunits;
	///////////////////////spoint  Light Profile
	//IES Texture;
	float IESIntensityScale;
	bool IESIntensity;
	

};

