#include "DiscLight.h"

DiscLight::DiscLight() :color({ 1.0f,1.0f,1.0f,1.0f }), Position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({ 0.f,0.f,0.f }),
sourceradius(0.0f), softsourceradius(0.0f), sourcelengh(0.0f), attenuationradius(0.0f), pad(0.0f)
{
}

DiscLight::~DiscLight()
{
}

DiscLight::DiscLight(XMFLOAT3 position, XMFLOAT3 rotaion) :Position(std::move(position)), Rotaion(std::move(rotation))
{
	color = { 1.0f,1.0f,1.0f,1.0f };
	Intensity = 8.0f;
	sourceradius = 0.0f;
	softsourceradius = 0.0f;
	sourcelengh = 0.0f;
	attenuationradius = 1000.0f;
	pad = 0.0f;
}

void DiscLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void DiscLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}
