#include "RectangleLight.h"

RectangleLight::RectangleLight() :color({ 1.0f,1.0f,1.0f,1.0f }), Position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({ 0.f,0.f,0.f }),
LightFront({ 1.0f,1.0f,1.0f,1.0f }), LightRight({ 1.0f,1.0f,1.0f,1.0f }), LightUp({ 1.0f,1.0f,1.0f,1.0f }), LightWidth(0.0f), LightHeight(0.0f)
{
}

RectangleLight::~RectangleLight()
{
}

RectangleLight::RectangleLight(XMFLOAT3 position, XMFLOAT3 rotation)
{
}

void RectangleLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void RectangleLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}
