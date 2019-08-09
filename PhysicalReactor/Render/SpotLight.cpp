#include "stdafx.h"
#include "SpotLight.h"


SpotLight::SpotLight() :colorAndIntensity({ 1.0f,1.0f,1.0f,8.0f }), OuterConeAngle(0.0f), Position({ 1.0f,1.0f,1.0f }), Rotaion({0.f,0.f,0.f}), InnerConeAngle(0.0f),  attenuationradius(0.0f), /*sourceradius(0.0f),*/ softsourceradius(0.f)/*,sourcelengh(0.0f)*/
                       /*,lightfalloffexponent(0.0f),IESIntensityScale(0.0f),intensityunits(Candelas),IESIntensity(false)
	                    ,useInverseSquaredFalloff(false),CastShadowformCinematicobjectonly(false)*/
{

}


SpotLight::~SpotLight()
{

}

SpotLight::SpotLight(XMFLOAT3 position, XMFLOAT3 rotaion) :Position(position), Rotaion(rotaion)
{
	colorAndIntensity = { 1.0f,1.0f,1.0f,100.0f };
	OuterConeAngle = 22.0f;
	
	InnerConeAngle = 0.0f;

	attenuationradius = 1000.0f;
	//sourceradius = 0.0f;
	softsourceradius = 0.0f;
	//sourcelengh = 0.0f;
	lightAngleScale=1.0f/PRE::fmax(0.001f,(PRE::fcos(InnerConeAngle) - PRE::fcos(OuterConeAngle)));
	lightAngleOffset=-PRE::fcos(OuterConeAngle) * lightAngleScale;
}

void SpotLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void SpotLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}
