#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////////////////
// Vector2 class
////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class Vector2 {
public:

	//=========================================================================================
	// constructor
	//=========================================================================================

	constexpr Vector2() = default;
	constexpr Vector2(T _x, T _y) : x(_x), y(_y) {}

	//=========================================================================================
	// compound assignment operator
	//=========================================================================================

	/* Add */
	constexpr Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }

	/* Subtract */
	constexpr Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }

	/* Multiply */
	constexpr Vector2& operator*=(const Vector2& v) { x *= v.x; y *= v.y; return *this; }
	constexpr Vector2& operator*=(T s) { x *= s; y *= s; return *this; }

	/* Division */
	constexpr Vector2& operator/=(const Vector2& v) { x /= v.x; y /= v.y; return *this; }
	constexpr Vector2& operator/=(T s) { x /= s; y /= s; return *this; }

	//=========================================================================================
	// cast operator
	//=========================================================================================

	template <typename U>
	constexpr operator Vector2<U>() const {
		return { static_cast<U>(x), static_cast<U>(y) };
	}

	//=========================================================================================
	// variables
	//=========================================================================================

	T x, y;

};

//=========================================================================================
// binary operator
//=========================================================================================

/* Add */
template <typename T>
constexpr Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2) {
	return { v1.x + v2.x, v1.y + v2.y };
}

/* Subtract */
template <typename T>
constexpr Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2) {
	return { v1.x - v2.x, v1.y - v2.y };
}

/* Multiply */
template <typename T>
constexpr Vector2<T> operator*(const Vector2<T>& v1, const Vector2<T>& v2) {
	return { v1.x * v2.x, v1.y * v2.y };
}

template <typename T>
constexpr Vector2<T> operator*(const Vector2<T>& v, T s) {
	return { v.x * s, v.y * s };
}

template <typename T>
constexpr Vector2<T> operator*(T s, const Vector2<T>& v) {
	return { s * v.x, s * v.y };
}

/* Division */
template <typename T>
constexpr Vector2<T> operator/(const Vector2<T>& v1, const Vector2<T>& v2) {
	return { v1.x / v2.x, v1.y / v2.y };
}

template <typename T>
constexpr Vector2<T> operator/(const Vector2<T>& v, T s) {
	return { v.x / s, v.y / s };
}

//=========================================================================================
// unary operator
//=========================================================================================

template <typename T>
constexpr Vector2<T> operator+(const Vector2<T> v) {
	return v;
}

template <typename T>
constexpr Vector2<T> operator-(const Vector2<T> v) {
	return { -v.x, -v.y };
}

//-----------------------------------------------------------------------------------------
// using
//-----------------------------------------------------------------------------------------

using Vector2f  = Vector2<float>;
using Vector2i  = Vector2<int32_t>;
using Vector2ui = Vector2<uint32_t>;