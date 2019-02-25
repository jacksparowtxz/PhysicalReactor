#include "stdafx.h"
#include "StaticMesh.h"


StaticMesh::StaticMesh():directory(""),aabb(nullptr)
{
	XMFLOAT3 zero = { 0,0,0 };
	XMFLOAT4 Id = { 0,0,0,1 };

	XMVECTOR vZero = XMLoadFloat3(&zero);
	XMVECTOR qId = XMLoadFloat4(&Id);

	XMFLOAT4 rot = {0,0,0,1};
	XMFLOAT3 pos = {0,0,0};
	XMFLOAT3 scale = { 0,0,0};
	XMVECTOR qRot = XMLoadFloat4(&rot);
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vScale = XMLoadFloat3(&scale);

	XMMATRIX mtx = XMMatrixTransformation(vZero, qId, vScale, vZero, qRot, vPos);
	mtx = XMMatrixTranspose(mtx);
	XMStoreFloat4x4(Transformation, mtx);

	drawkey = new DrawKey;



}


StaticMesh::~StaticMesh()
{
	SAFE_DELETE(aabb);
	
}

XMVECTOR StaticMesh::GetTransInformation(int index)
{
	XMVECTOR qRot;
	XMVECTOR vPos;
	XMVECTOR vScale;
	
	XMMATRIX WORLD;
	XMStoreFloat4x4(Transformation, WORLD);
	XMMatrixDecompose(&vScale, &qRot, &vPos, WORLD);
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

