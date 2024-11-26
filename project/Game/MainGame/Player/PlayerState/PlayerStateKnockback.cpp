#include "PlayerStateKnockback.h"

PlayerState::Knockback::Knockback(const Vector3f& direction_) :
	direction(direction_) {
}

void PlayerState::Knockback::begin() {
	timer.AddDeltaTime();
}

Vector3f PlayerState::Knockback::velocity() {
	constexpr float KNOCKBACK_TIME = 1.0f;
	constexpr float SPEED = 20.0f;
	float parametric = std::min(timer.time / KNOCKBACK_TIME, 1.0f);
	Vector3f result = direction * std::lerp(SPEED, 0.0f, parametric);
	if (parametric >= 1.0f) {
		isEnd = true;
	}
	return result;
}
