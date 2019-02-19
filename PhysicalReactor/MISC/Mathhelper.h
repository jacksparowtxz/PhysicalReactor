//***************************************************************************************
// MathHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper math class.
#pragma once
#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <Windows.h>
#include <DirectXmath.h>
#include <DirectXPackedVector.h>

#include <float.h>
#include <cmath>
using namespace DirectX;
using namespace DirectX::PackedVector;


#define saturate(x) min(max(x,0),1)


static const float PI = 3.14159265358979323846f;
static const float PI_TWO = 6.28318530717958647693f;
static const float PI_HALF = 1.57079632679489661923f;
static const float FLOAT_EPSILON = 1.0e-7f;


namespace PRE
{
	float Length(const XMFLOAT2& v);
	float Length(const XMFLOAT3& v);
	float Distance(const XMFLOAT2& v1, const XMFLOAT2& v2);
	float Distance(const XMFLOAT3& v1, const XMFLOAT3& v2);
	float DistanceSquared(const XMFLOAT3& v1, const XMFLOAT3& v2);
	float DistanceEstimated(const XMFLOAT3& v1, const XMFLOAT3& v2);
	float Distance(const XMVECTOR& v1, const XMVECTOR& v2);
	float DistanceSquared(const XMVECTOR& v1, const XMVECTOR& v2);
	float DistanceEstimated(const XMVECTOR& v1, const XMVECTOR& v2);
	XMFLOAT3 getVectorHalfWayPoint(const XMFLOAT3& a, const XMFLOAT3& b);
	bool Collision2D(const XMFLOAT2& hitBox1Pos, const XMFLOAT2& hitBox1Siz, const XMFLOAT2& hitBox2Pos, const XMFLOAT2& hitBox2Siz);
	float InverseLerp(float value1, float value2, float pos);
	float Lerp(float value1, float value2, float amount);
	XMFLOAT2 Lerp(const XMFLOAT2&, const XMFLOAT2&, float);
	XMFLOAT3 Lerp(const XMFLOAT3&, const XMFLOAT3&, float);
	XMFLOAT4 Lerp(const XMFLOAT4&, const XMFLOAT4&, float);
	XMFLOAT4 Slerp(const XMFLOAT4&, const XMFLOAT4&, float);
	XMFLOAT3 Max(const XMFLOAT3& a, const XMFLOAT3& b);
	XMFLOAT3 Min(const XMFLOAT3& a, const XMFLOAT3& b);
	float Clamp(float val, float min, float max);
	UINT GetNextPowerOfTwo(UINT x);
	float SmoothStep(float value1, float value2, float amount);

	// A, B, C: trangle vertices
	float TriangleArea(const XMVECTOR& A, const XMVECTOR& B, const XMVECTOR& C);
	// a, b, c: trangle side lengths
	float TriangleArea(float a, float b, float c);

	XMFLOAT3 getCubicHermiteSplinePos(const XMFLOAT3& startPos, const XMFLOAT3& endPos
		, const XMFLOAT3& startTangent, const XMFLOAT3& endTangent
		, float atInterval);
	XMFLOAT3 getQuadraticBezierPos(const XMFLOAT3& a, const XMFLOAT3&b, const XMFLOAT3& c, float t);
	XMFLOAT3 getQuadraticBezierPos(const XMFLOAT4& a, const XMFLOAT4&b, const XMFLOAT4& c, float t);

	XMFLOAT3 QuaternionToRollPitchYaw(const XMFLOAT4& quaternion);

	XMVECTOR GetClosestPointToLine(const XMVECTOR& A, const XMVECTOR& B, const XMVECTOR& P, bool segmentClamp = false);
	float GetPointSegmentDistance(const XMVECTOR& point, const XMVECTOR& segmentA, const XMVECTOR& segmentB);

	XMFLOAT3 HueToRGB(float H);
	float GetAngle(const XMFLOAT2& a, const XMFLOAT2& b);
	void ConstructTriangleEquilateral(float radius, XMFLOAT4& A, XMFLOAT4& B, XMFLOAT4& C);
	void GetBarycentric(const XMVECTOR& p, const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& c, float &u, float &v, float &w, bool clamp = false);

	// 64-iteration 4-dimensional Halton sequence (0 <= idx < 64)
	const XMFLOAT4& GetHaltonSequence(int idx);

	uint32_t CompressNormal(const XMFLOAT3& normal);
	uint32_t CompressColor(const XMFLOAT4& color);


	bool fequal(float a, float b, float epsilon = FLOAT_EPSILON);

	/// Returns the minimum of @a a and @a b.
	float fmin(float a, float b);

	/// Returns the maximum of @a a and @a b.
	float fmax(float a, float b);

	/// Clamps @a val to @a min and @a max.
	float fclamp(float val, float min, float max);

	/// Returns the fractional part of @a a.
	float ffract(float a);

	/// Returns the absolute value of @a a.
	float fabs(float a);

	/// Returns the sine of @a a.
	float fsin(float a);

	/// Returns the cosine of @a a.
	float fcos(float a);

	/// Returns the arc cosine of @a a.
	float facos(float a);

	/// Returns the tangent of @a a.
	float ftan(float a);

	/// Returns the nonnegative square root of @a a.
	float fsqrt(float a);

	/// Returns @a deg in radians.
	float frad(float deg);

	/// Returns @a rad in degrees.
	float fdeg(float rad);

	/// Returns the linear interpolated value between @a p0 and @a p1 at time @a t
	float lerp(const float p0, const float p1, float t);

	/// Returns the cosine interpolated value between @a p0 and @a p1 at time @a t
	float cosine(const float p0, const float p1, float t);

	/// Returns the cubic interpolated value between @a p0 and @a p1 at time @a t
	float cubic(const float p0, const float p1, float t);

	/// Bezier interpolation
	float bezier(const float p0, const float p1, const float p2, const float p3, float t);

	/// Catmull-Rom interpolation
	float catmull_rom(const float p0, const float p1, const float p2, const float p3, float t);

	template <class T>
	struct sRect1
	{
		static sRect1 Rect(T x, T y, T width, T height) {
			return sRect1<T>(x, y, x + width, y + height);
		}

		sRect1() : left(0), top(0), right(0), bottom(0) {
		}

		sRect1(T left, T top, T right, T bottom) {
			this->left = left;
			this->top = top;
			this->right = right;
			this->bottom = bottom;
		}

		bool IsIn(const T x, const T y) const {
			return (left <= x)
				&& (right >= x)
				&& (top <= y)
				&& (bottom >= y);
		}
		void SetX(const T x) {
			*this = sRect1<T>(x, top, x + Width(), bottom);
		}
		void SetY(const T y) {
			*this = sRect1(left, y, right, y + Height());
		}
		void SetWidth(const T width) {
			*this = Rect(left, top, width, Height());
		}
		void SetHeight(const T height) {
			*this = sRect1(left, top, right, height);
		}
		T Width() const {
			return abs(right - left);
		}
		T Height() const {
			return abs(bottom - top);
		}
		bool IsEmpty() const {
			return (0 == left) && (0 == right) && (0 == top) && (0 == bottom);
		}

		sRect1 operator-(const sRect1 &rhs) {
			return sRect1(left - rhs.left,
				top - rhs.top,
				right - rhs.right,
				bottom - rhs.bottom);
		}
		sRect1 operator+(const sRect1 &rhs) {
			return sRect1(left + rhs.left,
				top + rhs.top,
				right + rhs.right,
				bottom + rhs.bottom);
		}
		sRect1& operator=(const RECT &rhs) {
			left = (T)rhs.left;
			right = (T)rhs.right;
			top = (T)rhs.top;
			bottom = (T)rhs.bottom;
			return *this;
		}
		bool operator==(const sRect1 &rhs) {
			return (left == rhs.left)
				&& (right == rhs.right)
				&& (top == rhs.top)
				&& (bottom == rhs.bottom);
		}

		T left, top, right, bottom;
	};
	typedef sRect1<float> sRect;
};



class MathHelper
{
public:
	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF()*(b - a);
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a)*t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	static XMVECTOR RandUnitVec3();
	static XMVECTOR RandHemisphereUnitVec3(XMVECTOR n);

	static const float Infinity;
	static const float Pi;


};

#endif // MATHHELPER_H