#include "stdafx.h"
#include "RectangleLight.h"

RectangleLight::RectangleLight() :color({ 1.0f,1.0f,1.0f,1.0f }), Position({ 1.0f,1.0f,1.0f }), Intensity(10.f), Rotaion({ 0.f,0.f,0.f }), LightWidth(0.0f),
LightFront({ 1.0f,1.0f,1.0f}), LightHeight(0.0f), LightRight({ 1.0f,1.0f,1.0f }),pad(0.0), pad1({ 0.0f,0.0f,0.0f,0.0f }),direction({ 0.0f,0.0f,0.0f}),pad2(0.0f),pad3({ 0.0f,0.0f,0.0f,0.0f })
{

}

RectangleLight::~RectangleLight()
{
}

RectangleLight::RectangleLight(XMFLOAT3 position, XMFLOAT3 rotation) :Position(std::move(position)), Rotaion(std::move(rotation))
{



	color = { 1.0f,1.0f,1.0f,1.0f };
	Intensity = 8.0f;
	

	LightWidth = 2.0f;
	LightHeight = 1.0f;

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
	XMStoreFloat3(&LightFront, XMVector3TransformNormal(XMVectorSet(0, 0, -1, 0), W));
	XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), W));
}

void RectangleLight::SetPostion(XMFLOAT3 position)
{
	Position = position;
}

void RectangleLight::SetRotation(XMFLOAT3 rotation)
{
	Rotaion = rotation;
}
