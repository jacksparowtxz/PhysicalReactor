#include "stdafx.h"
#include"TubeLight.h"

TubeLight::TubeLight() :color({ 1.0f,1.0f,1.0f,1.0f }), Position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({ 0.f,0.f,0.f })
 ,attenuationradius(0.0f),LightRight({ 1.0f,1.0f,1.0f }), sourcewidth(5.0f)
{
}

TubeLight::~TubeLight()
{
}

TubeLight::TubeLight(XMFLOAT3 position, XMFLOAT3 rotation) :Position(std::move(position)), Rotaion(std::move(rotation))
{
	color = { 1.0f,1.0f,1.0f,1.0f };
	Intensity = 8.0f;
	
	sourcewidth = 5.0f;

	attenuationradius = 5.0f;
	XMFLOAT3 zero = { 0,0,0 };
	XMFLOAT4 Id = { 0,0,0,1 };
	XMFLOAT3 scale = { 1,1,1 };
	XMVECTOR vZero = DirectX::XMLoadFloat3(&zero);
	XMVECTOR qId = DirectX::XMLoadFloat4(&Id);

	XMVECTOR qRot = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z); 
	XMVECTOR vPos = DirectX::XMLoadFloat3(&position);
	XMVECTOR vScale = DirectX::XMLoadFloat3(&scale);

	XMMATRIX W = XMMatrixTransformation(vZero, qId, vScale, vZero, qRot, vPos);
	//W = XMMatrixTranspose(W);


	XMStoreFloat3(&LightRight, XMVector3TransformNormal(XMVectorSet(-1, 0, 0, 0), W));
}

void TubeLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void TubeLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}
