#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	//////////////Light
	float lightsourceangle;
	XMFLOAT4* ModulatedShadowscolor;
	bool castModulatedShadows;
	bool AtmosphereFogsunlight;



	/////////////////////////Directional shafts
	float occlusionmaskdrakness;
	float occlusiondepthrange;
	bool Lightshaftsocclusion;
	bool lightshaftbloom;
	bool bloomthreashold;
	XMFLOAT4* bloomtint;

	//////////////////////////////Directional field shadow
	float DistanceFieldShadowDistance;
	float distanceFieldTraceDistance;


	/////////////////////Directional Lightmass
	float LightSourceAngle;


	//////////////////////Dirtectional cascaded shadow map
	float dynamicshadowdistancemoveablelight;
	float dynamicshadowdistancestationarylight;
	float NumDynamicShadowcascaded;
	float DistrbutionExponent;
	float TransitionFraction;
	float Distancefadeoutfraction;
};

