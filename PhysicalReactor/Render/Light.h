#pragma once
#include"CommonInclude.h"



enum LightType
{
	DIRECTIONALLIGHT,
	POINTLIGHT,
	SPOTLIGHT,
    SKYLIGHT,
	NONE,
};

enum IntensityUnits
{
	Unitless,
	Candelas,
	Lumens,
};


enum SkySourceType
{
	SLSSpecifiedCubemap,
	SLSCapturedScene,
};

enum OcclusionCombineMode
{
	OCM_MIMIMUN,
	OCM_MULTIPLY,
};

class Light
{
public:
	Light();
	virtual ~Light();

	LightType lightype;
	

	//int Lightchannel;
	float Intensity;
	//float Temperature;
	//float IndirectlightingIntensity;
	//float volumetricScatteringIntensity;

	/*float MinRoughness;
	float shadowResolutionscale;
	float shadowbisa;
	float shadowflitersharpen;
	float contactshadowlength;

	bool Casttranslucentshadow;
	bool UseTemperature;
	bool Castshadow;
	bool dyanmicindirectLighting;
	bool Forcecachedshadowingformoveable;
	
	bool Caststaticshadow;
	bool Castdyanmicshadow;
	bool affecttranslucentLights;
	bool volumetricshadows;

	////////////////////shafts


	/////////////////////Lightmass
	
	float indirectLightingSaturation;
	float shadowExponent;
	bool useareashadowforstationarylight;


	////////////////////////perforamance
	float maxdrawdistance;
	float maxdistancefaderange;


	///////////////////////////light function
	//LightFunctionMaterial* LM;
	
	float fadedistance;
	float disabledBrightness;
	XMFLOAT4 *lightfunctionscale;*/
	

	
	XMFLOAT4X4* world;
	XMFLOAT4* color;
	









};

