#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////////////////
// Vector3 class
////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class Vector3 {
public:

	//=========================================================================================
	// constructor
	//=========================================================================================

	constexpr Vector3() = default;
	constexpr Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	//=========================================================================================
	// compound assignment operator
	//=========================================================================================

	/* Add */
	constexpr Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }

	/* Subtract */
	constexpr Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

	/* Multiply */
	constexpr Vector3& operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	constexpr Vector3& operator*=(T s) { x *= s; y *= s; z *= s; return *this; }

	/* Division */
	constexpr Vector3& operator/=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
	constexpr Vector3& operator/=(T s) { x /= s; y /= s; z /= s; return *this; }

	//=========================================================================================
	// cast opraotor
	//=========================================================================================

	template <typename U>
	constexpr operator Vector3<U>() const {
		return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(z) };
	}

	//=========================================================================================
	// variables
	//=========================================================================================

	T x, y, z;

};

//=========================================================================================
// binary operator
//=========================================================================================

/* Add */
template <typename T>
constexpr Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/* Subtract */
template <typename T>
constexpr Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

/* Multiply */
template <typename T>
constexpr Vector3<T> operator*(const Vector3<T>& v1, const Vector3<T>& v2) {
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

template <typename T>
constexpr Vector3<T> operator*(const Vector3<T>& v, T s) {
	return { v.x * s, v.y * s, v.z * s };
}

template <typename T>
constexpr Vector3<T> operator*(T s, const Vector3<T>& v) {
	return { s * v.x, s * v.y, s * v.z };
}

/* Division */
template <typename T>
constexpr Vector3<T> operator/(const Vector3<T>& v1, const Vector3<T>& v2) {
	return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}

template <typename T>
constexpr Vector3<T> operator/(const Vector3<T>& v, T s) {
	return { v.x / s, v.y / s, v.z / s };
}

//=========================================================================================
// unary operator
//=========================================================================================

template <typename T>
constexpr Vector3<T> operator+(const Vector3<T>& v) {
	return v;
}

template <typename T>
constexpr Vector3<T> operator-(const Vector3<T>& v) {
	return { -v.x, -v.y, -v.z };
}

//-----------------------------------------------------------------------------------------
// using
//-----------------------------------------------------------------------------------------

using Vector3f = Vector3<float>;
using Vector3ui = Vector3<uint32_t>;

//-----------------------------------------------------------------------------------------
// constexpr
//-----------------------------------------------------------------------------------------

constexpr const Vector3f kOrigin3 = { 0.0f, 0.0f, 0.0f };
constexpr const Vector3f kUnit3   = { 1.0f, 1.0f, 1.0f };

////////////////////////////////////////////////////////////////////////////////////////////
// Vector3 methods
////////////////////////////////////////////////////////////////////////////////////////////

Vector3f Normalize(const Vector3f& v);

float Length(const Vector3f& v);

float Dot(const Vector3f& x, const Vector3f& y);

Vector3f Cross(const Vector3f& x, const Vector3f& y);

Vector3f Reflect(const Vector3f& i, const Vector3f& n);

Vector3f Lerp(const Vector3f& x, const Vector3f& y, float s);

Vector3f Clamp(const Vector3f& v, const Vector3f& min, const Vector3f& max);