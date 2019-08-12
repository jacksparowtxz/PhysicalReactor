#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight() :color({ 1.0f,1.0f,1.0f,1.0f }), Position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({0.f,0.f,0.f}),
                          sourceradius(0.0f), softsourceradius(0.0f), sourcelengh(0.0f), attenuationradius(0.0f), pad(0.0f)//£¬intensityunits(Candelas)
                         /*,lightfalloffexponent(0.0f),useInverseSquaredFalloff(true),CastShadowformCinematicobjectonly(true),
	                     IESIntensityScale(0.0f),IESIntensity(false),raystartoffsetdepthscale(0.0f),RayTraceDistanceFieldshadows(0.0f)*/
{
	
}

PointLight::PointLight(XMFLOAT3 position, XMFLOAT3 rotation) :Position(std::move(position)), Rotaion(std::move(rotation))
{
	color = { 1.0f,1.0f,1.0f,1.0f };
	Intensity = 8.0f;
	sourceradius = 0.0f;
	softsourceradius = 0.0f;
	sourcelengh = 0.0f;
	attenuationradius = 1000.0f;
	pad = 0.0f;
}


PointLight::~PointLight()
{
}

void PointLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void PointLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}