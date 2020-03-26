#include "stdafx.h"
#include"SpereAreaLight.h"



SphereAreaLight::SphereAreaLight():color({ 1.0f,1.0f,1.0f,1.0f }), Position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({ 0.f,0.f,0.f }),
sourceradius(0.0f), softsourceradius(0.0f), sourcelengh(0.0f), attenuationradius(0.0f), pad(0.0f)
{

}

SphereAreaLight::SphereAreaLight(XMFLOAT3 position, XMFLOAT3 rotation) :Position(std::move(position)), Rotaion(std::move(rotation))
{
	color = { 1.0f,1.0f,1.0f,1.0f };
	Intensity = 1.0f;
	sourceradius = 0.0f;
	softsourceradius = 0.0f;
	sourcelengh = 0.0f;
	attenuationradius = 2.0f;
	pad = 0.0f;
}

SphereAreaLight::~SphereAreaLight()
{

}

void SphereAreaLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void SphereAreaLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}