#pragma once
#include "Light.h"
struct SpotLight
{

	SpotLight();
	~SpotLight();
	SpotLight(XMFLOAT3 Position, XMFLOAT3 Rotaion);
	//////////////////////////////SpotLight
	XMFLOAT3 color;
	float OuterConeAngle;
	XMFLOAT3 Position;
	float Intensity;
	XMFLOAT3 Rotaion;
	float InnerConeAngle;

	float attenuationradius;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
	

	void SetPostion(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
/*	float lightfalloffexponent;
	float IESIntensityScale;
	IntensityUnits intensityunits;
	///////////////////////spoint  Light Profile
	//IES Texture;
	
	bool IESIntensity;
	bool  useInverseSquaredFalloff;
	bool CastShadowformCinematicobjectonly;*/

};

//int i = sizeof(SpotLight);64