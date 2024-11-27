#include "BaseBossBehavior.h"

void BaseBossBehavior::begin() {
	if (isEnd) {
		return;
	}
	timer.AddDeltaTime();
}

void BaseBossBehavior::update_collider(const Vector3f& position) {
	if (attackCollider) {
		attackCollider->SetColliderPosition(position);
	}
}
