#include "stdafx.h"
#include "DiscLight.h"

DiscLight::DiscLight() :color({ 1.0f,1.0f,1.0f,1.0f }), Position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({ 0.f,0.f,0.f }),
attenuationradius(0.0f), Front({ 1.0f,1.0f,1.0f }), pad(0.0f)
{
}

DiscLight::~DiscLight()
{
}

DiscLight::DiscLight(XMFLOAT3 position, XMFLOAT3 rotation) :Position(std::move(position)), Rotaion(std::move(rotation))
{
	XMFLOAT3 zero = { 0,0,0 };
	XMVECTOR qid = XMQuaternionIdentity();
	XMFLOAT4 scale = { 1,1,1,0 };
	XMVECTOR vZero = DirectX::XMLoadFloat3(&zero);
	//XMVECTOR qId = DirectX::XMLoadFloat4(&Id);

	XMVECTOR qRot = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z); // DirectX::XMLoadFloat4(&rot);
	XMVECTOR vPos = DirectX::XMLoadFloat3(&position);
	XMVECTOR vScale = DirectX::XMLoadFloat4(&scale);
	
	XMMATRIX W = XMMatrixTransformation(g_XMZero, qid, vScale, vZero, qRot, vPos);
	
	//W = XMMatrixTranspose(W);

	color = { 1.0f,1.0f,1.0f,1.0f };
	Intensity = 1.0f;
	XMStoreFloat3(&Front, XMVector3TransformNormal(XMVectorSet(0, 0, -1, 0), W));
	attenuationradius = 5.0f;
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
