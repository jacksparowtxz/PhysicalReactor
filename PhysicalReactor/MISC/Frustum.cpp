/////////////////
#include "Frustum.h"
#include "Intersectables.h"


Frustum::Frustum()
{


}

void Frustum::CleanUp()
{
	
}

void Frustum::ConstructFrustum(float screenDepth, XMFLOAT4X4 projectionMatrix, const XMFLOAT4X4& viewMatrix, const XMMATRIX& world /* = XMMatrixIdentity() */)
{
	view = viewMatrix;
	float zMinimum;
	float r;
	XMFLOAT4X4 matrix;

	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r*zMinimum;

	XMStoreFloat4x4(&matrix, XMMatrixMultiply(XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix), world), XMLoadFloat4x4(&projectionMatrix)));
	m_planes[0].x = matrix._14 + matrix._13;
	m_planes[0].y = matrix._24 + matrix._23;
	m_planes[0].z = matrix._34 + matrix._33;
	m_planes[0].w = matrix._44 + matrix._43;
	XMStoreFloat4(&m_planeNorm[0], XMPlaneNormalize(XMLoadFloat4(&m_planes[0])));

	// Calculate far plane of frustum.
	m_planes[1].x = matrix._14 - matrix._13;
	m_planes[1].y = matrix._24 - matrix._23;
	m_planes[1].z = matrix._34 - matrix._33;
	m_planes[1].w = matrix._44 - matrix._43;
	XMStoreFloat4(&m_planeNorm[1], XMPlaneNormalize(XMLoadFloat4(&m_planes[1])));

	// Calculate left plane of frustum.
	m_planes[2].x = matrix._14 + matrix._11;
	m_planes[2].y = matrix._24 + matrix._21;
	m_planes[2].z = matrix._34 + matrix._31;
	m_planes[2].w = matrix._44 + matrix._41;
	XMStoreFloat4(&m_planeNorm[2], XMPlaneNormalize(XMLoadFloat4(&m_planes[2])));

	// Calculate right plane of frustum.
	m_planes[3].x = matrix._14 - matrix._11;
	m_planes[3].y = matrix._24 - matrix._21;
	m_planes[3].z = matrix._34 - matrix._31;
	m_planes[3].w = matrix._44 - matrix._41;
	XMStoreFloat4(&m_planeNorm[3], XMPlaneNormalize(XMLoadFloat4(&m_planes[3])));

	// Calculate top plane of frustum.
	m_planes[4].x = matrix._14 - matrix._12;
	m_planes[4].y = matrix._24 - matrix._22;
	m_planes[4].z = matrix._34 - matrix._32;
	m_planes[4].w = matrix._44 - matrix._42;
	XMStoreFloat4(&m_planeNorm[4], XMPlaneNormalize(XMLoadFloat4(&m_planes[4])));

	// Calculate bottom plane of frustum.
	m_planes[5].x = matrix._14 + matrix._12;
	m_planes[5].y = matrix._24 + matrix._22;
	m_planes[5].z = matrix._34 + matrix._32;
	m_planes[5].w = matrix._44 + matrix._42;
	XMStoreFloat4(&m_planeNorm[5], XMPlaneNormalize(XMLoadFloat4(&m_planes[5])));
}

bool Frustum::CheckPoint(const XMFLOAT3& point) const
{
	XMVECTOR p = XMLoadFloat3(&point);
	for (short i=0;i<6;i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planeNorm[i]), p)) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool  Frustum::CheckSphere(const XMFLOAT3& center, float radius) const
{
	XMVECTOR c = XMLoadFloat3(&center);
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planeNorm[i]), c)) < -radius)
		{
			return false;
		}

	}
	return true;
}

int Frustum::CheckBox(const AABB& box) const
{
	int iTotalIn = 0;
	for (int p=0;p<6;++p)
	{
		int iInCount = 8;
		int iPtIn = 1;

		for (int i=0;i<8;++i)
		{
			if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planeNorm[p]), XMLoadFloat3(&box.corners[i]))) < 0.0f)
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		if (iInCount==0)
		{
			return 0;
		}
		iTotalIn += iPtIn;
	}
	if (iTotalIn==6)
	{
		return BOX_FRUSTUM_INSIDE;
	}
	return BOX_FRUSTUM_INTERSECTS;
}

const XMFLOAT4& Frustum::getLeftPlane()const
{

	return m_planeNorm[2];
}

const XMFLOAT4& Frustum::getRightPlane() const
{ 
	return m_planeNorm[3]; 
}
const XMFLOAT4& Frustum::getTopPlane() const 
{ 
	return m_planeNorm[4]; 
}
const XMFLOAT4& Frustum::getBottomPlane() const
{
	return m_planeNorm[5];
}
const XMFLOAT4& Frustum::getFarPlane() const
{
	return m_planeNorm[1]; 
}
const XMFLOAT4& Frustum::getNearPlane() const 
{ 
	return m_planeNorm[0];
}

DirectX::XMFLOAT3 Frustum::getCamPos() const
{
	return XMFLOAT3(-view._41, -view._42, -view._43);
}

