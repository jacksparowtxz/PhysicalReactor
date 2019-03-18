#include "stdafx.h"
#include "SpotLight.h"


SpotLight::SpotLight() :color({ 1.0f,1.0f,1.0f }), OuterConeAngle(0.0f), position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({0.f,0.f,0.f}), InnerConeAngle(0.0f),  attenuationradius(0.0f), sourceradius(0.0f), softsourceradius(0.f),sourcelengh(0.0f)
                       /*,lightfalloffexponent(0.0f),IESIntensityScale(0.0f),intensityunits(Candelas),IESIntensity(false)
	                    ,useInverseSquaredFalloff(false),CastShadowformCinematicobjectonly(false)*/
{

}


SpotLight::~SpotLight()
{

}

SpotLight::SpotLight(XMFLOAT3 position, XMFLOAT3 rotaion) :Position(position), Rotaion(rotaion)
{
	color = { 1.0f,1.0f,1.0f };
	OuterConeAngle = 0.0f;
	Intensity = 1.0f;
	InnerConeAngle = 0.0f;

	attenuationradius = 0.0f;
	sourceradius = 0.0f;
	softsourceradius = 0.0f;
	sourcelengh = 0.0f;
}

void SpotLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void SpotLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}
