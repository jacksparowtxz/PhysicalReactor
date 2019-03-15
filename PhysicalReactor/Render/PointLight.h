#pragma once
#include "Light.h"
class PointLight
	
{
public:
	PointLight();
	~PointLight();

	/////////////////////point
	//IntensityUnits intensityunits;
	float attenuationradius;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
	BaseLightInfo Light;

/*	float lightfalloffexponent;
	bool  useInverseSquaredFalloff;
	bool CastShadowformCinematicobjectonly;

	
	///////////////////////point  Light Profile
	//IES Texture;
	float IESIntensityScale;
	bool IESIntensity;
	

	////////////////////////////// field shadow
	float raystartoffsetdepthscale;///P
	bool RayTraceDistanceFieldshadows;/// P */
	
};


