#include "stdafx.h"
#include "SpotLight.h"


SpotLight::SpotLight():InnerConeAngle(0.0f),OuterConeAngle(0.0f),attenuationradius(0.0f),sourceradius(0.0f),sourcelengh(0.0f)
                       ,lightfalloffexponent(0.0f),IESIntensityScale(0.0f),intensityunits(Candelas),IESIntensity(false)
	                    ,useInverseSquaredFalloff(false),CastShadowformCinematicobjectonly(false)
{

}


SpotLight::~SpotLight()
{

}
