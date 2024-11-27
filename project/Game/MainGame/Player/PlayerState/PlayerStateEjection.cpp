#include "PlayerStateEjection.h"

PlayerState::Ejection::Ejection(const Vector3f& direction_, float ejectDistance) :
	direction(direction_), Time(ejectDistance / 10.0f){
	create_collider();
}

void PlayerState::Ejection::begin() {
	timer.AddDeltaTime();
	if (timer.time >= Time) {
		isEnd = true;
	}
}

Vector3f PlayerState::Ejection::velocity() {
	return direction * 15.0f;
}
