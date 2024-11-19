#include "PlayerStateEjection.h"

PlayerState::Ejection::Ejection(const Vector3f& direction_) : direction(direction_) {
}

void PlayerState::Ejection::begin() {
	timer.AddDeltaTime();
	if (timer.time >= 1.0f) {
		isEnd = true;
	}
}

Vector3f PlayerState::Ejection::velocity() {
	return direction * 10.0f;
}
