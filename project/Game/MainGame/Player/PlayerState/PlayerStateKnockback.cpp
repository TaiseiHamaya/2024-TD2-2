#include "PlayerStateKnockback.h"

PlayerState::Knockback::Knockback(const Vector3f& direction_) :
	direction(direction_) {
}

void PlayerState::Knockback::begin() {
	timer.AddDeltaTime();
}

Vector3f PlayerState::Knockback::velocity() {
	float parametric = timer.time / 3.0f;
	Vector3f result = direction * std::lerp(1.0f, 0.0f, std::min(parametric, 1.0f));
	if (parametric >= 1.0f) {
		isEnd = true;
	}
	return result;
}
