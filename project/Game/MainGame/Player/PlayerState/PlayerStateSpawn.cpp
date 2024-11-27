#include "PlayerStateSpawn.h"

#include <Engine/System/Performance.h>

PlayerState::Spawn::Spawn() {
	startY = 15.0f;
	velocityY = 0;
	displacement = 0;
}

void PlayerState::Spawn::begin() {
	velocityY -= 9.8f * Performance::GetDeltaTime(s).time;
}

Vector3f PlayerState::Spawn::velocity() {
	Vector3f result{ 0, velocityY, 0 };
	displacement += velocityY * Performance::GetDeltaTime(s).time;
	if (startY <= -displacement) {
		isEnd = true;
	}
	return result;
}
