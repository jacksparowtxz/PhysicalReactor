#pragma once
#include "Light.h"
struct PointLight
{

	PointLight();
	~PointLight();

	/////////////////////point
	//IntensityUnits intensityunits;
	XMFLOAT4 color;
	XMFLOAT3 position;
	float Intensity;
	XMFLOAT3 Rotaion;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
	float attenuationradius;
	float pad;

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


//int i = sizeof(PointLight);64