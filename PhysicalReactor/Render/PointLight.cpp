#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight():intensityunits(Candelas),attenuationradius(0.0f),sourceradius(0.0f),softsourceradius(0.0f),
                         lightfalloffexponent(0.0f),useInverseSquaredFalloff(true),CastShadowformCinematicobjectonly(true),
	                     IESIntensityScale(0.0f),IESIntensity(false),raystartoffsetdepthscale(0.0f),RayTraceDistanceFieldshadows(0.0f)
{
	lightype = POINTLIGHT;
}


PointLight::~PointLight()
{
}
