#include "Vector2.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////
// Vector2 methods
////////////////////////////////////////////////////////////////////////////////////////////

float Length(const Vector2f& v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

Vector2f Normalize(const Vector2f& v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y);

	if (length == 0.0f) {
		return {}; //!< ゼロ除算対策
	}

	return v / length;
}

float Dot(const Vector2f& x, const Vector2f& y) {
	return x.x * y.x + x.y * y.y;
}

float Cross(const Vector2f& x, const Vector2f& y) {
	return x.x * y.y - x.y * y.x;
}

Vector2f Lerp(const Vector2f& x, const Vector2f& y, float t) {
	return {
		std::lerp(x.x, y.x, t),
		std::lerp(x.y, y.y, t),
	};
}
