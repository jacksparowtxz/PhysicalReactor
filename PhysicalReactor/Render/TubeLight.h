#pragma once
#include"Light.h"


struct TubeLight
{
	TubeLight();
	~TubeLight();
	TubeLight(XMFLOAT3 position, XMFLOAT3 rotation);

	XMFLOAT4 color;
	XMFLOAT3 Position;
	float Intensity;
	XMFLOAT3 Rotaion;
	float attenuationradius;
	float sourceradius;
	float sourcewidth;
	float sourcelengh;
	float pad;
	void SetPostion(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);




};