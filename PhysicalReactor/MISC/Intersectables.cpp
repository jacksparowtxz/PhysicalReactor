#include "Intersectables.h"
#include "mathhelper.h"

AABB::AABB()
{
   for (int i=0;i<8;++i)
   {
	   corners[i] = XMFLOAT3(0, 0, 0);
   }
}

AABB::AABB(const XMFLOAT3&min, const XMFLOAT3& max)
{
	create(min, max);
}

void AABB::createFromHalfWidth(const XMFLOAT3& center, const XMFLOAT3& halfwidth)
{
	XMFLOAT3 min = XMFLOAT3(center.x - halfwidth.x, center.y - halfwidth.y, center.z - halfwidth.z);
	XMFLOAT3 max = XMFLOAT3(center.x + halfwidth.x, center.y + halfwidth.y, center.z + halfwidth.z);
	create(min, max);
}

void AABB::create(const XMFLOAT3& min, const XMFLOAT3& max)
{
	corners[0] = min;
	corners[1] = XMFLOAT3(min.x, max.y, min.z);
	corners[2] = XMFLOAT3(min.x, max.y, max.z);
	corners[3] = XMFLOAT3(min.x, min.y, max.z);
	corners[4] = XMFLOAT3(max.x, min.y, min.z);
	corners[5] = XMFLOAT3(max.x, max.y, min.z);
	corners[6] = max;
	corners[7] = XMFLOAT3(max.x, min.y, max.z);
}

AABB AABB::get(const XMMATRIX& mat)
{

	AABB ret;
	XMFLOAT3 min;
	XMFLOAT3 max;
	for (int i=0;i<8;++i)
	{
		XMVECTOR point = XMVector3Transform(XMLoadFloat3(&corners[i]), mat);
		XMStoreFloat3(&ret.corners[i], point);
	}

	min = ret.corners[0];
	max = ret.corners[6];
	for (int i=0;i<8;++i)
	{
		XMFLOAT3& p = ret.corners[i];
		if (p.x < min.x) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;

		if (p.x > max.x) max.x = p.x;
		if (p.y > max.y) max.y = p.y;
		if (p.z > max.z) max.z = p.z;

	}
	ret.create(min, max);
	return ret;
}

AABB AABB::get(const XMFLOAT4X4& mat)
{
	return get(XMLoadFloat4x4(&mat));
}

XMFLOAT3 AABB::getMin() const
{
	return corners[0];
}
XMFLOAT3 AABB::getMax() const
{
	return corners[6];
}

XMFLOAT3 AABB::getCenter() const
{
	XMFLOAT3 min = getMin();
	XMFLOAT3 max = getMax();

	return XMFLOAT3((min.x + max.x)*0.5f, (min.y + max.y)*0.5f, (min.z + max.z)*0.5f);

}

XMFLOAT3 AABB::getHalfWidth() const
{
	XMFLOAT3 max = getMax();
	XMFLOAT3 center = getCenter();
	return XMFLOAT3(abs(max.x - center.x), abs(max.y - center.y), abs(max.z - center.z));
}

XMMATRIX AABB::getAsBoxMatrix() const
{
	XMFLOAT3 ext = getHalfWidth();
	XMMATRIX sca = XMMatrixScaling(ext.x, ext.y, ext.z);
	XMFLOAT3 pos = getCenter();
	XMMATRIX tra = XMMatrixTranslation(pos.x, pos.y, pos.z);

	return sca*tra;
}

float AABB::getArea() const
{
	XMFLOAT3 min = getMin();
	XMFLOAT3 max = getMax();
	return (max.x - min.x)*(max.y - min.y)*(max.z - min.z);


}


float AABB::getRadius() const
{
	XMFLOAT3 abc = getHalfWidth();
	return max(max(abc.x, abc.y), abc.z);
}

AABB::INTERSECTION_TYPE AABB::intersects(const AABB& b) const
{
	XMFLOAT3 aMin = getMin(), aMax = getMax();
	XMFLOAT3 bMin = b.getMin(), bMax = b.getMax();

	if (bMin.x >= aMin.x && bMax.x <= aMax.x &&
		bMin.y >= aMin.y && bMax.y <= aMax.y &&
		bMin.z >= aMin.z && bMax.z <= aMax.z)
	{
		return INSIDE;
	}

	if (aMax.x < bMin.x || aMin.x > bMax.x)
		return OUTSIDE;
	if (aMax.y < bMin.y || aMin.y > bMax.y)
		return OUTSIDE;
	if (aMax.z < bMin.z || aMin.z > bMax.z)
		return OUTSIDE;

	return INTERSECTS;
    

}

bool AABB::intersects(const XMFLOAT3& p) const
{
	XMFLOAT3 max = getMax();
	XMFLOAT3 min = getMin();
	if (p.x > max.x) return false;
	if (p.x < min.x) return false;
	if (p.y > max.y) return false;
	if (p.y < min.y) return false;
	if (p.z > max.z) return false;
	if (p.z < min.z) return false;
	return true;
}

bool AABB::intersects(const RAY& ray) const 
{
	if (intersects(ray.origin))
		return true;

	XMFLOAT3 MIN = getMin();
	XMFLOAT3 MAX = getMax();

	float tx1 = (MIN.x - ray.origin.x)*ray.direction_inverse.x;
	float tx2 = (MAX.x - ray.origin.x)*ray.direction_inverse.x;

	float tmin = min(tx1, tx2);
	float tmax = max(tx1, tx2);

	float ty1 = (MIN.y - ray.origin.y)*ray.direction_inverse.y;
	float ty2 = (MAX.y - ray.origin.y)*ray.direction_inverse.y;

	tmin = max(tmin, min(ty1, ty2));
	tmax = min(tmax, max(ty1, ty2));

	float tz1 = (MIN.z - ray.origin.z)*ray.direction_inverse.z;
	float tz2 = (MAX.z - ray.origin.z)*ray.direction_inverse.z;

	tmin = max(tmin, min(tz1, tz2));
	tmax = min(tmax, max(tz1, tz2));

	return tmax >= tmin;
}

AABB AABB::operator *(float a)
{
	XMFLOAT3 min = getMin();
	XMFLOAT3 max = getMax();
	min.x *= a;
	min.y *= a;
	min.z *= a;
	max.x *= a;
	max.y *= a;
	max.z *= a;
	return AABB(min, max);
}

AABB AABB::Merge(const AABB& a, const AABB&b)
{
	return AABB(PRE::Min(a.getMin(), b.getMin()), PRE::Max(a.getMax(), b.getMax()));
}

/*
void AABB::Serialize(Archive& archive)
{
	if (archive.IsReadMode())
	{
		archive >> corners[0];
		archive >> corners[1];
		archive >> corners[2];
		archive >> corners[3];
		archive >> corners[4];
		archive >> corners[5];
		archive >> corners[6];
		archive >> corners[7];
	}
	else
	{
		archive << corners[0];
		archive << corners[1];
		archive << corners[2];
		archive << corners[3];
		archive << corners[4];
		archive << corners[5];
		archive << corners[6];
		archive << corners[7];
	}



}*/

bool SPHERE::intersects(const AABB& b)const
{
	XMFLOAT3 min = b.getMin();
	XMFLOAT3 max = b.getMax();
	XMFLOAT3 closestPointInAABB = PRE::Min(PRE::Max(center, min), max);
	double distanceSquared = PRE::Distance(closestPointInAABB, center);
	return distanceSquared < radius;
}

bool SPHERE::intersects(const SPHERE& b) const
{
	return PRE::Distance(center, b.center) <= radius + b.radius;
}

bool SPHERE::intersects(const RAY& b) const
{
	XMVECTOR o = XMLoadFloat3(&b.origin);
	XMVECTOR r = XMLoadFloat3(&b.direction);
	XMVECTOR dist = XMVector3LinePointDistance(o, o + r, XMLoadFloat3(&center));
	return XMVectorGetX(dist) <= radius;
}

bool RAY::intersects(const AABB&b) const 
{
	return b.intersects(*this);
}

bool RAY::intersects(const SPHERE& b)const
{
	return b.intersects(*this);
 }



bool Hitbox2D::intersects(const Hitbox2D& b)
{
	return PRE::Collision2D(pos, siz, b.pos, b.siz);
}


