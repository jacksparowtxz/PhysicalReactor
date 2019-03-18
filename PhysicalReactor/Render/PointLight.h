#pragma once
#include "Light.h"
struct PointLight
{

	PointLight();
	PointLight(XMFLOAT3 position, XMFLOAT3 rotaion);
	~PointLight();

	/////////////////////point
	//IntensityUnits intensityunits;
	XMFLOAT4 color;
	XMFLOAT3 Position;
	float Intensity;
	XMFLOAT3 Rotaion;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
	float attenuationradius;
	float pad;
	void SetPostion(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
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