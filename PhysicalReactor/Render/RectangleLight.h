#pragma once
#include"Light.h"

struct RectangleLight
{

	RectangleLight();
	~RectangleLight();
	RectangleLight(XMFLOAT3 position, XMFLOAT3 rotation);


	XMFLOAT4 color;
	XMFLOAT3 Position;
	float Intensity;
	XMFLOAT3 Rotaion;
	float LightWidth;
	XMFLOAT3 LightFront;
	float LightHeight;
	XMFLOAT3 LightRight;
	float pad;
	XMFLOAT4 pad1;
	XMFLOAT3 direction;
	float pad2;
	XMFLOAT4 pad3;
	void SetPostion(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);

};

