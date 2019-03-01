#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	PointLight();
	~PointLight();

	/////////////////////point
	IntensityUnits intensityunits;
	float attenuationradius;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
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

