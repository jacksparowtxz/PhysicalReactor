#pragma once
#include "Light.h"
struct SpotLight
{

	SpotLight();
	~SpotLight();
	SpotLight(XMFLOAT3 Position, XMFLOAT3 Rotaion);
	//////////////////////////////SpotLight
	XMFLOAT4 colorAndIntensity;
	XMFLOAT3 Position;
	float InnerConeAngle;
	XMFLOAT3 Rotaion;
	float OuterConeAngle;
	float attenuationradius;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
	float lightAngleScale; //lightAngleScale=1.0f/max(0.001f,(cosInner-cosOuter));
	float lightAngleOffset; //lightAngleOffset=-cosOuter*angleScale;
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

int i = sizeof(SpotLight);