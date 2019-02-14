//////////////////////
#pragma once
#include "CommonInclude.h"

struct AABB;

class Frustum
{
private:
	XMFLOAT4 m_planeNorm[6];
	XMFLOAT4 m_planes[6];
	XMFLOAT4X4 view;


public:
	Frustum();
	void CleanUp();
	void ConstructFrustum(float screenDepth,  XMFLOAT4X4 projectionMatrix, const XMFLOAT4X4& viewMatrix,
		const XMMATRIX& world = XMMatrixIdentity());

	bool CheckPoint(const XMFLOAT3& point) const;

	bool CheckSphere(const XMFLOAT3& center, float radius)const;
	int CheckBox(const AABB& box)const;

#define BOX_FRUSTUM_INTERSECTS 1
#define BOX_FRUSTUM_INSIDE 2

	const XMFLOAT4& getLeftPlane()const;
	const XMFLOAT4& getRightPlane() const;
	const XMFLOAT4& getTopPlane() const;
	const XMFLOAT4& getBottomPlane() const;
	const XMFLOAT4& getFarPlane() const;
	const XMFLOAT4& getNearPlane() const;

	XMFLOAT3 getCamPos()const;
};