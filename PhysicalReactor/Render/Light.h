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

