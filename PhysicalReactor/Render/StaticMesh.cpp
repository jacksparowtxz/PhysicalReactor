#include "stdafx.h"
#include "StaticMesh.h"


StaticMesh::StaticMesh() :directory(""), aabb(nullptr)
{
	XMFLOAT3 zero = { 0,0,0 };
	XMFLOAT4 Id = { 0,0,0,1 };

	XMVECTOR vZero = DirectX::XMLoadFloat3(&zero);
	XMVECTOR qId = DirectX::XMLoadFloat4(&Id);

	XMFLOAT4 rot = {0,0,0,1};
	XMFLOAT3 pos = {0,0,0};
	XMFLOAT3 scale = { 1,1,1};
	XMVECTOR qRot = DirectX::XMLoadFloat4(&rot);
	XMVECTOR vPos = DirectX::XMLoadFloat3(&pos);
	XMVECTOR vScale = DirectX::XMLoadFloat3(&scale);

	XMMATRIX mtx = XMMatrixTransformation(vZero, qId, vScale, vZero, qRot, vPos);
	mtx = XMMatrixTranspose(mtx);
	Transformation = new XMFLOAT4X4;
	DirectX::XMStoreFloat4x4(Transformation, mtx);

	drawkey = new DrawKey;



}

StaticMesh::StaticMesh(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale) :directory(""), aabb(nullptr)
{
	XMFLOAT3 zero = { 0,0,0 };
	XMFLOAT4 Id = { 0,0,0,1 };

	XMVECTOR vZero = DirectX::XMLoadFloat3(&zero);
	XMVECTOR qId = DirectX::XMLoadFloat4(&Id);

	XMVECTOR qRot = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z); // DirectX::XMLoadFloat4(&rot);
	XMVECTOR vPos = DirectX::XMLoadFloat3(&pos);
	XMVECTOR vScale = DirectX::XMLoadFloat3(&scale);

	XMMATRIX mtx = XMMatrixTransformation(vZero, qId, vScale, vZero, qRot, vPos);
	mtx = XMMatrixTranspose(mtx);
	Transformation = new XMFLOAT4X4;
	DirectX::XMStoreFloat4x4(Transformation, mtx);

	drawkey = new DrawKey;
}


StaticMesh::~StaticMesh()
{
	SAFE_DELETE(aabb);
	SAFE_DELETE(drawkey);
	SAFE_DELETE(Transformation);
	
	for (auto sm : Meshs)
	{
		sm->~SubMesh();
		SAFE_DELETE(sm);
	}
}

XMVECTOR StaticMesh::GetTransInformation(int index)
{
	XMVECTOR qRot;
	XMVECTOR vPos;
	XMVECTOR vScale;
	
	XMMATRIX WORLD;
	DirectX::XMStoreFloat4x4(Transformation, WORLD);
	DirectX::XMMatrixDecompose(&vScale, &qRot, &vPos, WORLD);
	if (index == 1)
	{
		return vPos;
	}
	else if (index == 2)
	{
		return qRot;
	}
	else
	{
		return vScale;
	}
}

BitSizeStruct<31> StaticMesh::GetMaterialID()
{
	return *Meshs[0]->material->MaterialID;
}

bool StaticMesh::GetMaterialBlendMode()
{
	if (Meshs[0]->material->GetBlendMode() == BLENDMODE::BLEND_OPAQUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

