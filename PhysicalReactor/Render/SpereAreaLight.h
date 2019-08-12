#pragma once
#include "Light.h"

struct SphereAreaLight
{
	SphereAreaLight();
	SphereAreaLight(XMFLOAT3 position, XMFLOAT3 rotation);
	~SphereAreaLight();

	XMFLOAT4 color;
	XMFLOAT3 Position;
	float Intensity;
	XMFLOAT3 Rotaion;
	float attenuationradius;
	float sourceradius;
	float softsourceradius;
	float sourcelengh;
	float pad;
	void SetPostion(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);




};