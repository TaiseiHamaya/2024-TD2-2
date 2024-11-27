#include "BossBehaviorKnockback.h"

#include <Engine/System/Performance.h>

#include "../Boss.h"

BossBehaviorKnockback::BossBehaviorKnockback(const Vector3f direction_) :
	direction(direction_) {
}

void BossBehaviorKnockback::move() {
	constexpr float KNOCKBACK_TIME = 3.0f;
	constexpr float SPEED = 10.0f;
	float parametric = std::min(timer.time / KNOCKBACK_TIME, 1.0f);
	Vector3f velocity = direction * std::lerp(SPEED, 0.0f, parametric);
	Vector3f& translate = boss->get_transform().transform.translate;
	translate += velocity * Performance::GetDeltaTime(s).time;
	float length = Length(translate);
	if (length >= 38.0f) {
		Vector3f normalized = Normalize(translate);
		translate = normalized * 38.0f;
	}
	if (timer.time >= 1.0f) {
		isEnd = true;
	}
}
