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
	XMFLOAT3 LightUp;
	float pad1;
	XMFLOAT4 pad2;
	XMFLOAT4 pad3;
	void SetPostion(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);

};

