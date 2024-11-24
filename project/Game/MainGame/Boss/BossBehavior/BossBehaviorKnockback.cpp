#include "BossBehaviorKnockback.h"

#include <Engine/System/Performance.h>

#include "../Boss.h"

BossBehaviorKnockback::BossBehaviorKnockback(const Vector3f direction_) :
	direction(direction_) {
}

void BossBehaviorKnockback::move() {
	boss->get_transform().transform.translate += direction * Performance::GetDeltaTime(s).time;
	if (timer.time >= 1.0f) {
		isEnd = true;
	}
}
