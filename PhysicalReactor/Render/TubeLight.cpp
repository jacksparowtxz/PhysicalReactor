#include "stdafx.h"
#include"TubeLight.h"

TubeLight::TubeLight() :color({ 1.0f,1.0f,1.0f,1.0f }), Position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({ 0.f,0.f,0.f }),
sourceradius(0.0f), sourcewidth(5.0f), sourcelengh(10.0f), attenuationradius(0.0f), pad(0.0f)
{

}

TubeLight::~TubeLight()
{
}

TubeLight::TubeLight(XMFLOAT3 position, XMFLOAT3 rotation) :Position(std::move(position)), Rotaion(std::move(rotation))
{
	color = { 1.0f,1.0f,1.0f,1.0f };
	Intensity = 8.0f;
	sourceradius = 0.0f;
	sourcewidth = 5.0f;
	sourcelengh = 10.0f;
	attenuationradius = 1000.0f;
	pad = 0.0f;
}

void TubeLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void TubeLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}
