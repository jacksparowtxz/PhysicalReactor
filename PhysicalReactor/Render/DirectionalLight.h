#pragma once
#include "Light.h"
class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

	XMFLOAT4 color;
	XMFLOAT3 direction;
	float Intensity;
	
	//////////////Light
/*	float lightsourceangle;
	
	


	/////////////////////////Directional shafts
	float occlusionmaskdrakness;
	float occlusiondepthrange;

	

	//////////////////////////////Directional field shadow
	float DistanceFieldShadowDistance;
	float DistanceFieldTraceDistance;


	/////////////////////Directional Lightmass
	float LightSourceAngle;


	//////////////////////Dirtectional cascaded shadow map
	float dynamicshadowdistancemoveablelight;
	float dynamicshadowdistancestationarylight;
	float NumDynamicShadowcascaded;
	float DistrbutionExponent;
	float TransitionFraction;
	float Distancefadeoutfraction;

	bool castModulatedShadows;
	bool AtmosphereFogsunlight;
	bool Lightshaftsocclusion;
	bool lightshaftbloom;
	bool bloomthreashold;

	XMFLOAT4* ModulatedShadowscolor;
	XMFLOAT4* bloomtint; */
};

//int i = sizeof(DirectionalLight);32